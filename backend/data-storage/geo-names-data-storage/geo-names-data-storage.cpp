#include "geo-names-data-storage.h"

namespace GeoNames
{
    GeoNamesDataStorage::GeoNamesDataStorage(QObject *parent)
        : QObject{parent}
    {}

    GeoNamesDataStorage::GeoNamesDataStorage(QStringList countryList, QObject *parent)
        : QObject{parent}
    {
        for (const auto &item : countryList)
        {
            _countryLocationsMap[item] = {};
        }
    }

    QVariantMap GeoNamesDataStorage::getCountryData(const QString &countryCode) const
    {
        QVariantMap countryData;
        countryData["key"] = countryCode;
        countryData["mainTitle"] = countryCode;

        const auto listForItteration = _countryLocationsMap[countryCode];

        QVariantList cityList;

        for (const auto &data : listForItteration)
        {
            QVariantMap listItem;
            listItem["key"] = QString("%1_%2").arg(data.latitude, data.longitude);
            listItem["mainTitle"] = data.name;
            listItem["secondTitle"] = data.adminName;
            listItem["data"] = data.toVariantMap();

            cityList.append(listItem);
        }

        countryData["items"] = cityList;

        return countryData;
    }

    QStringList GeoNamesDataStorage::getCountryList() const
    {
        return _countryLocationsMap.keys();
    }

    bool GeoNamesDataStorage::addData(const QPair<QString, QList<GeoNames::GeoParsingData>> &data)
    {
        const auto &[countryCode, locationList] = data;

        if (!_countryLocationsMap.contains(countryCode))
        {
            _countryLocationsMap[countryCode] = locationList;

            return true;
        }

        const auto newDataLength = locationList.length();
        const auto geoParsingDataList = _countryLocationsMap[countryCode];

        QSet<GeoParsingData> uniqueSet;

        uniqueSet.unite(QSet<GeoParsingData>{locationList.constBegin(), locationList.constEnd()});
        uniqueSet.unite(QSet<GeoParsingData>{geoParsingDataList.constBegin(), geoParsingDataList.constEnd()});

        _countryLocationsMap[countryCode] = uniqueSet.values();

        return newDataLength != geoParsingDataList.length();
    }
}
