#ifndef TEMPERATURE_DATA_H
#define TEMPERATURE_DATA_H

namespace WeatherForecast
{
    struct TemperatureData
    {
        int temperature;          // celcium.
        int feelsLike; // celcium.
        int min;       // celcium.
        int max;       // celcium.

        /**
         * @brief Convert to json view.
         * @return QVariantMap serialized GeoParsingData.
         */
        QVariantMap toVariantMap() const;

        /**
         * @brief Get GeoParsingData by parsing json View.
         * @param data - QVariantMap serialized GeoParsingData.
         * @return Reconstructed CountryLocationsData object.
         */
        static TemperatureData getByVariantMap(const QVariantMap &data);

        static TemperatureData getByVariantMapInApiResponse(const QVariantMap &data);
    };
}
#endif // TEMPERATURE_DATA_H
