#ifndef WEATHER_FORECAST_CACHE_SERVICE_H
#define WEATHER_FORECAST_CACHE_SERVICE_H


#include "../../../core/entities/weather-forecast/forecast-data/forecast-data.h"
#include "../interfaces/i-weather-forecast-cache-service.h"


namespace WeatherForecast
{
    /**
     * @brief In-memory cache service for weather forecast data.
     */
    class WeatherForecastCacheService : public QObject, public IWeatherForecastCacheService
    {
        Q_OBJECT
    public:
        /**
         * @brief Create storage.
         * @param parent - parent object.
         */
        explicit WeatherForecastCacheService(QObject *parent = nullptr);

        /**
         * @brief Retrieve cached forecast data for location.
         * @param key - Location identifier (e.g., city name, coordinates).
         * @return Optional containing forecast data if found.
         */
        std::optional<ForecastData> get(const QString& key) override;

        /**
         * @brief Store forecast data for location.
         * @param key - Location identifier (e.g., city name, coordinates).
         * @param data - Forecast data to cache.
         */
        void set(const QString& key, ForecastData data) override;

        /**
         * @brief Remove forecast data for location.
         * @param key - Location identifier to remove from cache.
         */
        void remove(const QString& key) override;

        /**
         * @brief Clear all cached forecast data.
         */
        void clear() override;

        /**
         * @brief Check if forecast data exists for location.
         * @param key - Location identifier to check.
         * @return True if forecast data is cached for this location.
         */
        bool contains(const QString& key) override;
    private:
        /**
         * @brief In-memory storage for weather forecast data.
         *
         * Key: Location identifier (city name, coordinates, or unique ID).
         * Value: Cached forecast data for the location.
         */
        QMap<QString, ForecastData> _weatherForecastMap;
    };
}

#endif // WEATHER_FORECAST_CACHE_SERVICE_H
