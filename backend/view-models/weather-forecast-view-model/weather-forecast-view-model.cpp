#include "weather-forecast-view-model.h"


namespace WeatherForecast
{
    WeatherForecastViewModel::WeatherForecastViewModel(
        WeatherForecast::WeatherForecastDataProvider*  dataProvider,
        QObject* parent
    ) : QObject(parent),
        _dataProvider(dataProvider)
    {
        connect(
            _dataProvider,
            &WeatherForecast::WeatherForecastDataProvider::weatherForecastFetched,
            this,
            &WeatherForecast::WeatherForecastViewModel::handleDataFetched
        );
    }

    void WeatherForecastViewModel::fetchDataAsync(const QVariantMap &requestData)
    {
        RequestData preparatedRequest = RequestData::createFromQVariantMap(requestData);

        _dataProvider->fetchDataByRequestAsync(std::move(preparatedRequest));
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

    void WeatherForecastViewModel::handleDataFetched(const WeatherForecast::ForecastData &forecastData)
    {
        emit weatherFetched(
            forecastData.toVariantMap()
        );
    }
}
