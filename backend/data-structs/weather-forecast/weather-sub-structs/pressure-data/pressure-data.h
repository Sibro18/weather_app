#ifndef PRESSURE_DATA_H
#define PRESSURE_DATA_H

namespace WeatherForecast
{
    struct PressureData
    {
        double groundLevel; // hPa.
        double seaLevel;    // hPa.

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
        static PressureData getByVariantMap(const QVariantMap &data);

        static PressureData getByVariantMapInApiResponse(const QVariantMap &data);
    };
}

#endif // PRESSURE_DATA_H
