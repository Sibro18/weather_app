#ifndef PRESSURE_DATA_H
#define PRESSURE_DATA_H


namespace WeatherForecast
{
    /**
     * @brief Atmospheric pressure measurements at different altitudes.
     */
    struct PressureData
    {
        double groundLevel; ///< Atmospheric pressure at ground level [hPa].
        double seaLevel;    ///< Atmospheric pressure at sea level [hPa].

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
        static PressureData getByVariantMap(const QVariantMap &data);

        /**
         * @brief Parse PressureData from external API response format.
         * @param data - QVariantMap from API response containing pressure data.
         * @return Parsed PressureData object.
         */
        static PressureData getByVariantMapInApiResponse(const QVariantMap &data);
    };
}

#endif // PRESSURE_DATA_H
