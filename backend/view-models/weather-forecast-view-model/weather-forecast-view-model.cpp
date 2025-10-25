#include <QPointer>
#include <chrono>
#include <algorithm>

#include "weather-forecast-view-model.h"


namespace WeatherForecast
{
    WeatherForecastViewModel::WeatherForecastViewModel(
        WeatherApiController* apiController,
        GeneralUtils::TaskManager* taskManager,
        WeatherForecast::WeatherForecastDataStorage* dataStorage,
        QObject* parent
    ) : QObject(parent),
        _apiController(apiController),
        _taskManager(taskManager),
        _dataStorage(dataStorage)
    {
        connect(
            _apiController,
            &WeatherForecast::WeatherApiController::weatherFetched,
            this,
            &WeatherForecast::WeatherForecastViewModel::handleDataFromApiFetched
        );
    }

    void WeatherForecastViewModel::fetchDataAsync(QVariantMap requestData)
    {
        auto invokeFetchWeatherFromByApi = [] (QPointer<WeatherForecastViewModel> controller, RequestData request) {
            QMetaObject::invokeMethod(controller, [controller = std::move(controller), request = std::move(request)]() {
                if (!controller)
                {
                    return;
                }

                controller->_apiController->fetchWeather(request);
            }, Qt::QueuedConnection);
        };

        _taskManager->runAsync(
            GeneralUtils::Priority::High,
            [
                controller = QPointer<WeatherForecastViewModel>(this),
                requestData = std::move(requestData),
                invokeFetchWeatherFromByApi = std::move(invokeFetchWeatherFromByApi)
            ] ()
            {
                RequestData preparatedRequest = RequestData::createFromQVariantMap(
                    std::move(requestData)
                );

                QString cacheKey = QString("%1_%2")
                    .arg(preparatedRequest.latitude, preparatedRequest.longitude);

                auto data = controller->_dataStorage.getForecasetData(cacheKey);

                // request to Data storage and return if exists

                if (data != std::nullopt)
                {
                    const ForecastData& forecastData = data.value();

                    if (controller->_verifyOnExpired(forecastData, preparatedRequest.resource))
                    {
                        emit controller->weatherFetched(forecastData.toVariantMap());

                        return;
                    }
                }

                invokeFetchWeatherFromByApi(std::move(controller), std::move(preparatedRequest));
            }
        );
    }

    int WeatherForecastViewModel::getAvgTemperatureOffTheDay(const QVariantList &variantList) const
    {
        if (variantList.length() == 0)
        {
            return INT_MIN;
        }

        auto&& [minTemperature, maxTemperature] = _getMinMaxTemperatures(WeatherData::getByVariantList(variantList));

        return (minTemperature + maxTemperature) / 2;
    }

    QVariantMap WeatherForecastViewModel::getAvgTemperatureRange(const QVariantList &variantList) const
    {
        if (variantList.isEmpty())
        {
            return {{"min", 0}, {"max", 0}};
        }

        QVariantMap returnData;
        auto [minTemperature, maxTemperature] = _getMinMaxTemperatures(WeatherData::getByVariantList(variantList));

        returnData["min"] = minTemperature;
        returnData["max"] = maxTemperature;

        return returnData;
    }

    QPair<int, int> WeatherForecastViewModel::_getMinMaxTemperatures(QList<WeatherData> data) const
    {
        int minTemperature = INT_MAX;
        int maxTemperature = INT_MIN;

        for (const WeatherData& item : data) {
            minTemperature = std::min(minTemperature, item.temperatureData.min);
            maxTemperature = std::max(maxTemperature, item.temperatureData.max);
        }

        return std::make_pair(minTemperature, maxTemperature);
    }

    bool WeatherForecastViewModel::_verifyOnExpired(const ForecastData& forecastData, const QString& requestResource) const
    {
        using namespace std::chrono;

        auto now = system_clock::now();
        std::time_t now_t = system_clock::to_time_t(now);
        std::tm* local_tm = std::localtime(&now_t);

        // Текущие компоненты даты
        int current_year = local_tm->tm_year + 1900;
        int current_month = local_tm->tm_mon + 1;
        int current_day = local_tm->tm_mday;
        int current_hour = local_tm->tm_hour;

        if (requestResource == "weather" && forecastData.hourForecast != std::nullopt)
        {
            const auto& hourForecast = forecastData.hourForecast.value();

            // Предполагаем, что hourForecast.first - это system_clock::time_point
            std::time_t forecast_t = system_clock::to_time_t(hourForecast.first);
            std::tm* forecast_tm = std::localtime(&forecast_t);

            int forecast_year = forecast_tm->tm_year + 1900;
            int forecast_month = forecast_tm->tm_mon + 1;
            int forecast_day = forecast_tm->tm_mday;
            int forecast_hour = forecast_tm->tm_hour;

            // Проверяем совпадение всех компонентов
            return (current_year == forecast_year &&
                current_month == forecast_month &&
                current_day == forecast_day &&
                current_hour == forecast_hour
            );
        }
        else if (forecastData.fiveDaysForecast != std::nullopt)
        {
            const auto& fiveDaysForecast = forecastData.fiveDaysForecast.value();

            // Предполагаем, что fiveDaysForecast.first - это system_clock::time_point
            std::time_t forecast_t = system_clock::to_time_t(fiveDaysForecast.first);
            std::tm* forecast_tm = std::localtime(&forecast_t);

            int forecast_year = forecast_tm->tm_year + 1900;
            int forecast_month = forecast_tm->tm_mon + 1;
            int forecast_day = forecast_tm->tm_mday;

            // Проверяем совпадение даты
            return (current_year == forecast_year &&
                current_month == forecast_month &&
                current_day == forecast_day
            );
        }

        return false;
    }

    void WeatherForecastViewModel::handleDataFromApiFetched(WeatherForecast::FetchResult* fetchResult)
    {
        QString cacheKey = QString("%1_%2")
            .arg(fetchResult->requestData.latitude, fetchResult->requestData.longitude);

        qDebug() << cacheKey;

        _dataStorage.addForecastData(
            std::make_pair(
                cacheKey,
                ForecastData(fetchResult->data, std::chrono::system_clock::now())
            )
        );

        auto variantMap = _dataStorage.getForecasetData(cacheKey).value().toVariantMap();

        emit weatherFetched(
            std::move(variantMap)
        );
    }
}
