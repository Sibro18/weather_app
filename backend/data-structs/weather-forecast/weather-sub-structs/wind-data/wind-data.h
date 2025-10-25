#ifndef WIND_DATA_H
#define WIND_DATA_H

namespace WeatherForecast
{
    struct WindData
    {
        double speed; // meter/sec.
        double degrees; // degrees.
        double gust; // meter/sec

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
        static WindData getByVariantMap(const QVariantMap &data);

        static WindData getByVariantMapInApiResponse(const QVariantMap &data);
    };
}

#endif // WIND_DATA_H
