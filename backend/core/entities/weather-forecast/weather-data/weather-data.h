#ifndef WEATHER_DATA_H
#define WEATHER_DATA_H


#include "../../weather-forecast/weather-sub-structs/weather-short-description/weather-short-description.h"
#include "../../weather-forecast/weather-sub-structs/wind-data/wind-data.h"
#include "../../weather-forecast/weather-sub-structs/temperature-data/temperature-data.h"
#include "../../weather-forecast/weather-sub-structs/pressure-data/pressure-data.h"


namespace WeatherForecast
{
    /**
     * @brief Weather data container.
     */
    struct WeatherData
    {
        /**
         * @brief Default constructor.
         */
        WeatherData() = default;

        WeatherForecast::WeatherShortDescription weatherShortDescription;  ///< Short weather condition.
        QString longDescription;                                           ///< Human-readable weather description.

        TemperatureData temperatureData; ///< Temperature data container.
        PressureData pressureData;       ///< Pressure data container.
        WindData windData;               ///< Wind data container.

        double clouds;      ///< Cloudiness percentage [0-100%].
        double humidity;    ///< Humidity percentage [0-100%].
        double visibility;  ///< Visibility distance in meters.

        std::optional<QPair<double, QString>> rain; ///< Rainfall amount (mm) and period.
        std::optional<QPair<double, QString>> snow; ///< Snowfall amount (mm) and period.

        qint64 unixTime;                      ///< UTC timestamp in seconds since epoch.
        std::optional<QString> dateIsoFormat; ///< ISO 8601 formatted date string.

        /**
         * @brief Parse weather data from API response object.
         * @param data - API response variant map.
         * @return Parsed WeatherData object.
         */
        static WeatherData parseWeatherByApiResponseObj(const QVariantMap& data);

        /**
         * @brief Parse list of weather data from API response array.
         * @param dataList - API response variant list.
         * @return List of parsed WeatherData objects.
         */
        static QList<WeatherData> parseWeatherListByApiResponse(const QVariantList& dataList);

        /**
         * @brief Convert to json view.
         * @return QVariantMap serialized LocationData.
         */
        QVariantMap toVariantMap() const;

        /**
         * @brief Get LocationData by parsing json View.
         * @param data - QVariantMap serialized LocationData.
         * @return Reconstructed CountryLocationsData object.
         */
        static WeatherData getByVariantMap(const QVariantMap &data);

        /**
         * @brief Reconstruct list of WeatherData from serialized list.
         * @param dataList - QVariantList containing serialized weather data.
         * @return List of reconstructed WeatherData objects.
         */
        static QList<WeatherData> getByVariantList(const QVariantList& dataList);
    };
}

#endif // WEATHER_DATA_H
