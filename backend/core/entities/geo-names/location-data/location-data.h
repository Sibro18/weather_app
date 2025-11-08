#ifndef LOCATION_DATA_H
#define LOCATION_DATA_H


#include <QJsonObject>

#include "../../../base/json-serializable.h"


namespace GeoNames
{
    /**
     * @brief Geographic location parsing data.
     */
    struct LocationData : public JsonSerializable<LocationData>
    {
        /**
         * @brief Default constructor.
         */
        LocationData() = default;

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
         * of this LocationData instance.
         *
         * @param obj - QJsonObject containing geographic data fields.
         */
        void fillFromJsonObject(const QJsonObject& obj);

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
        static LocationData convertToGeoParsingData(const QVariantMap &data);

        /**
         * @brief Redefinition equlity operator, for possibility create QSet.
         * @param other - LocationData.
         * @return true if equal, false otherwise.
         */
        bool operator==(const LocationData& other) const;
    };

    /**
     * @brief Computes a hash value for LocationData objects for use in QHash-based containers.
     * @param data - LocationData.
     * @param seed - Optional seed value for the hash computation.
     * @return The computed hash value.
     */
    inline size_t qHash(const LocationData& data, size_t seed = 0)
    {
        return qHash(data.latitude, seed)
            ^ qHash(data.longitude, seed);
    }
}

#endif // LOCATION_DATA_H
