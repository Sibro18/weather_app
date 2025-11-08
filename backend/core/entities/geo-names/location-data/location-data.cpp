#include "location-data.h"

namespace GeoNames
{
    QJsonObject LocationData::toJson() const
    {
        QJsonObject obj;
        obj["adminName1"] = adminName;
        obj["countryName"] = countryName;
        obj["countryCode"] = countryCode;
        obj["toponymName"] = toponymName;
        obj["name"] = name;
        obj["lat"] = latitude;
        obj["lng"] = longitude;

        return obj;
    }

    void LocationData::fillFromJsonObject(const QJsonObject &obj)
    {
        countryName = obj.value("countryName").toString();
        countryCode = obj.value("countryCode").toString();
        toponymName = obj.value("toponymName").toString();
        name = obj.value("name").toString();

        adminName = obj.contains("adminName1")
         ? obj.value("adminName1").toString()
         : obj.value("adminName").toString();
        latitude = obj.contains("lat")
         ? obj.value("lat").toString()
         : obj.value("latitude").toString();
        longitude = obj.contains("lng")
         ? obj.value("lng").toString()
         : obj.value("longitude").toString();
    }

    QVariantMap LocationData::toVariantMap() const
    {
        return {
            {"adminName", adminName},
            {"countryName", countryName},
            {"countryCode", countryCode},
            {"toponymName", toponymName},
            {"name", name},
            {"latitude", latitude},
            {"longitude", longitude}
        };
    }

    LocationData LocationData::convertToGeoParsingData(const QVariantMap &data)
    {
        LocationData result;

        result.adminName = data["adminName"].toString();
        result.countryName = data["countryName"].toString();
        result.countryCode = data["countryCode"].toString();
        result.toponymName = data["toponymName"].toString();
        result.name = data["name"].toString();
        result.latitude = data["latitude"].toString();
        result.longitude = data["longitude"].toString();

        return result;
    }

    bool LocationData::operator==(const LocationData& other) const
    {
        return longitude == other.longitude
            && latitude == other.latitude;
    }
}
