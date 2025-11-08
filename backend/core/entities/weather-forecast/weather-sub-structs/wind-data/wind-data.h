#ifndef WIND_DATA_H
#define WIND_DATA_H


namespace WeatherForecast
{
    /**
     * @brief Wind measurements including speed, direction, and gusts.
     */
    struct WindData
    {
        double speed;   ///< Wind speed [m/s].
        double degrees; ///< Wind direction in meteorological degrees [0-360].
        double gust;    ///< Wind gust speed [m/s].

        /**
         * @brief Convert wind data to JSON-serializable map.
         * @return QVariantMap representation for serialization.
         */
        QVariantMap toVariantMap() const;

        /**
         * @brief Reconstruct WindData from internal serialized map.
         * @param data - QVariantMap containing serialized wind data.
         * @return Reconstructed WindData object.
         */
        static WindData getByVariantMap(const QVariantMap &data);

        /**
         * @brief Parse WindData from external API response format.
         * @param data - QVariantMap from API response containing wind data.
         * @return Parsed WindData object.
         */
        static WindData getByVariantMapInApiResponse(const QVariantMap &data);
    };
}

#endif // WIND_DATA_H
