#include "weather-forecast-data-provider.h"


namespace WeatherForecast
{
    WeatherForecastDataProvider::WeatherForecastDataProvider(
        WeatherForecast::IWeatherApiController* apiController,
        WeatherForecast::IWeatherForecastCacheService* cacheService,
        QObject* parent
    ) : QObject(parent),
        _apiController(apiController),
        _cacheService(cacheService)
    {
        connect(
            _apiController,
            &IWeatherApiController::dataFetched,
            this,
            &WeatherForecast::WeatherForecastDataProvider::handleDataFromApiFetched
        );
    }

    void WeatherForecastDataProvider::fetchDataByRequestAsync(WeatherForecast::RequestData requestData)
    {
        QString cacheKey = QString("%1_%2")
            .arg(requestData.latitude, requestData.longitude);

        auto data = _cacheService->get(cacheKey);

        if (!data) {
            _apiController->fetchData(std::move(requestData));
            return;
        }

        emit weatherForecastFetched(*data);
    }

    void WeatherForecastDataProvider::handleDataFromApiFetched(const WeatherForecast::FetchResult& fetchResult)
    {
        QString cacheKey = QString("%1_%2")
            .arg(fetchResult.requestData.latitude, fetchResult.requestData.longitude);

        _cacheService->set(
            cacheKey,
            ForecastData(fetchResult.data, std::chrono::system_clock::now())
        );

        emit weatherForecastFetched(*_cacheService->get(cacheKey));
    }
}
