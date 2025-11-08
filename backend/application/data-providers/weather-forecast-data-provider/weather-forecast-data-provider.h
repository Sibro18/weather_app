#ifndef WEATHER_FORECAST_DATA_PROVIDER_H
#define WEATHER_FORECAST_DATA_PROVIDER_H


#include "../../../infrastructure/api/interfaces/i-weather-api-controller.h"
#include "../../../infrastructure/cache/interfaces/i-weather-forecast-cache-service.h"


namespace WeatherForecast
{
    /**
     * @brief Coordinates weather data flow between API controller, cache, and application components.
     */
    class WeatherForecastDataProvider : public QObject
    {
        Q_OBJECT
    public:
        /**
         * @brief Constructs weather forecast data provider.
         * @param apiController - Weather API controller for external data fetching.
         * @param cacheService - Cache service for data storage and retrieval.
         * @param parent - Optional parent QObject.
         */
        explicit WeatherForecastDataProvider(
            WeatherForecast::IWeatherApiController* apiController,
            WeatherForecast::IWeatherForecastCacheService* cacheService,
            QObject* parent = nullptr
        );

        /**
         * @brief Fetches WeatherForecast data for the given request parameters.
         * @param requestData - Parameters specifying the request for WeatherForecast data.
         */
        void fetchDataByRequestAsync(WeatherForecast::RequestData requestData);
    signals:
        /**
         * @brief Emitted when WeatherForecast data has been fetched and parsed.
         * @param fetchResult - The result of the fetch operation.
         */
        void weatherForecastFetched(const WeatherForecast::ForecastData &forecastFetchedData);
    private:
        WeatherForecast::IWeatherApiController* _apiController;       ///< API data source.
        WeatherForecast::IWeatherForecastCacheService* _cacheService; ///< Data cache layer.
    private slots:
        /**
         * @brief Handles WeatherForecast data received from the API controller.
         * @param fetchResult - The result returned from the API call.
         */
        void handleDataFromApiFetched(const WeatherForecast::FetchResult& fetchResult);
    };
}

#endif // WEATHER_FORECAST_DATA_PROVIDER_H
