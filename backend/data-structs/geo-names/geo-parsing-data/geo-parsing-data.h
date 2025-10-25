#ifndef GEO_PARSING_DATA_H
#define GEO_PARSING_DATA_H

#include "../../../utils//json-serializable/json-serializable.h"
#include <QString>
#include <QJsonObject>
#include <QVariantMap>


namespace GeoNames
{
    /**
     * @brief Geographic location parsing data.
     */
    struct GeoParsingData : public JsonSerializable<GeoParsingData>
    {
        GeoParsingData() = default;

        /**
         * @brief Administrative area name (e.g., state, province, region).
         */
        QString adminName;

        /**
         * @brief Full country name.
         */
        QString countryName;

        /**
         * @brief ISO country code (e.g., "US", "RU").
         */
        QString countryCode;

        /**
         * @brief Official toponym or geographic feature name.
         */
        QString toponymName;

        /**
         * @brief Commonly used name of the location.
         */
        QString name;

        /**
         * @brief Latitude coordinate as a string.
         */
        QString latitude;

        /**
         * @brief Longitude coordinate as a string.
         */
        QString longitude;

        /**
         * @brief Converts this object into a QJsonObject.
         *
         * @return QJsonObject containing the serialized field data.
         */
        QJsonObject toJson() const;

        /**
         * @brief Populates this object from a QJsonObject.
         *
         * Reads and assigns JSON fields to the corresponding members
         * of this GeoParsingData instance.
         *
         * @param obj - QJsonObject containing geographic data fields.
         */
        void fillFromJsonObject(const QJsonObject& obj);

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
        static GeoParsingData convertToGeoParsingData(const QVariantMap &data);

        /**
         * @brief Redefinition equlity operator, for possibility create QSet.
         * @param other - GeoParsingData.
         * @return true if equal, false otherwise.
         */
        bool operator==(const GeoParsingData& other) const;
    };

    /**
     * @brief Computes a hash value for GeoParsingData objects for use in QHash-based containers.
     * @param data - GeoParsingData.
     * @param seed - Optional seed value for the hash computation.
     * @return The computed hash value.
     */
    inline size_t qHash(const GeoParsingData& data, size_t seed = 0)
    {
        return qHash(data.latitude, seed)
            ^ qHash(data.longitude, seed);
    }
}

#endif // GEO_PARSING_DATA_H
