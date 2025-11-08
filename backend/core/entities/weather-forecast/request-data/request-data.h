#ifndef WEATHER_FORECAST_REQUEST_DATA_H
#define WEATHER_FORECAST_REQUEST_DATA_H


namespace WeatherForecast
{
    /**
     * @brief Weather API resource types with bidirectional string conversion.
     */
    struct WeatherResource
    {
        enum Type
        {
            CurrentWeather, ///< Current weather conditions.
            FiveDayForecast ///< 5-day weather forecast.
        };

        /**
         * @brief Converts resource type to API endpoint string.
         */
        inline static QString toString(Type resource)
        {
            static const QHash<Type, QString> nameMap = {
                {CurrentWeather, "weather"},
                {FiveDayForecast, "forecast"}
            };

            return nameMap.value(resource);
        }

        /**
         * @brief Converts string to resource type.
         */
        inline static Type fromString(const QString& str)
        {
            if (str == "weather")
            {
                return CurrentWeather;
            }

            if (str == "forecast")
            {
                return FiveDayForecast;
            }

            return CurrentWeather;
        }
    };

    /**
     * @brief Parameters for making a request to the OpenWeather API.
     */
    struct RequestData
    {
        /**
         * @brief Type of weather data to request ("weather" or "forecast").
         */
        WeatherResource::Type resource;

        /**
         * @brief Latitude of the location.
         */
        QString latitude = "";

        /**
         * @brief Longitude of the location.
         */
        QString longitude = "";

        /**
         * @brief Get request data from json view.
         * @param requestData - json view.
         * @return RequestData.
         */
        static RequestData createFromQVariantMap(const QVariantMap &requestData);

        /**
         * @brief Convert to json view.
         * @return Json view.
         */
        QVariantMap toVariantMap() const;
    };
}

#endif // WEATHER_FORECAST_REQUEST_DATA_H
