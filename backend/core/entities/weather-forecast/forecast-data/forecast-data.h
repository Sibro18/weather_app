#ifndef FORECAST_DATA_H
#define FORECAST_DATA_H


#include <chrono>

#include "../weather-data/weather-data.h"
#include "../request-data/request-data.h"


namespace WeatherForecast
{
    /**
     * @brief Container for weather forecast data with caching timestamps.
     */
    struct ForecastData
    {
        /**
         * @brief Default constructor.
         */
        ForecastData() = default;

        /**
         * @brief Constructs forecast data from weather data list and timestamp.
         * @param data - List of weather data points.
         * @param timePoint - Timestamp when data was received.
         */
        ForecastData(const QList<WeatherData>& data, const std::chrono::system_clock::time_point& timePoint);

        /**
         * @brief 5-day forecast data with timestamp of when it was fetched.
         * @note Contains timestamp and list of weather data points.
         */
        std::optional<QPair<std::chrono::system_clock::time_point, QList<WeatherData>>> fiveDaysForecast;

        /**
         * @brief Hourly forecast data with timestamp of when it was fetched.
         * @note Contains timestamp and single weather data point.
         */
        std::optional<QPair<std::chrono::system_clock::time_point, WeatherData>> hourForecast;

        /**
         * @brief Converts forecast data to QVariantMap for serialization.
         * @return Serialized representation of the forecast data.
         */
        QVariantMap toVariantMap() const;

        /**
         * @brief Validates if forecast data is complete for the requested resource type.
         * @param requestResource - Type of weather resource to validate against.
         * @return True if data is valid and complete for the given resource type.
         */
        bool isValid(const WeatherResource::Type& requestResource) const;
    };
}

#endif // FORECAST_DATA_H
