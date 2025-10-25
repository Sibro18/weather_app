#ifndef WEATHER_FORECAST_REQUEST_DATA_H
#define WEATHER_FORECAST_REQUEST_DATA_H


#include <QString>


namespace WeatherForecast
{
    /**
     * @brief Parameters for making a request to the OpenWeather API.
     */
    struct RequestData
    {
        /**
         * @brief Type of weather data to request ("weather" or "forecast").
         */
        QString resource = "";

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
