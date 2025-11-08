#ifndef TEMPERATURE_DATA_H
#define TEMPERATURE_DATA_H


namespace WeatherForecast
{
    /**
     * @brief Temperature measurements in Celsius.
     */
    struct TemperatureData
    {
        int temperature; ///< Actual temperature [°C].
        int feelsLike;   ///< Perceived temperature (wind chill/heat index) [°C].
        int min;         ///< Minimum temperature [°C].
        int max;         ///< Maximum temperature [°C].

        /**
         * @brief Convert to json view.
         * @return QVariantMap serialized LocationData.
         */
        QVariantMap toVariantMap() const;

        /**
         * @brief Reconstruct TemperatureData from internal serialized map.
         * @param data - QVariantMap containing serialized temperature data.
         * @return Reconstructed TemperatureData object.
         */
        static TemperatureData getByVariantMap(const QVariantMap &data);

        /**
         * @brief Parse TemperatureData from external API response format.
         * @param data - QVariantMap from API response containing temperature data.
         * @return Parsed TemperatureData object.
         */
        static TemperatureData getByVariantMapInApiResponse(const QVariantMap &data);
    };
}
#endif // TEMPERATURE_DATA_H
