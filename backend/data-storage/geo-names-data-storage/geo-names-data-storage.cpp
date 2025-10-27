#include <QVariantList>
#include <QVariantMap>
#include <QSet>

#include "geo-names-data-storage.h"


namespace GeoNames
{
    GeoNamesDataStorage::GeoNamesDataStorage(QObject *parent)
        : QObject{parent}
    {}

    GeoNamesDataStorage::GeoNamesDataStorage(const QStringList &countryList, QObject *parent)
        : QObject{parent}
    {
        for (const auto &item : countryList)
        {
            _countryLocationsMap[item] = {};
        }
    }

    QVariantMap GeoNamesDataStorage::getCountryData(const QString &countryCode) const
    {
        auto it = _countryLocationsMap.constFind(countryCode);

        if (it == _countryLocationsMap.constEnd())
        {
            return {};
        }

        QVariantMap countryData;
        countryData["key"] = countryCode;
        countryData["mainTitle"] = countryCode;

        QVariantList cityList;
        cityList.reserve(it->size());

        for (const auto &data : *it)
        {
            QVariantMap listItem;
            listItem["key"] = QString("%1_%2").arg(data.latitude, data.longitude);
            listItem["mainTitle"] = data.name;
            listItem["secondTitle"] = data.adminName;
            listItem["data"] = data.toVariantMap();

            cityList.append(listItem);
        }

        countryData["items"] = std::move(cityList);

        return countryData;
    }

    QStringList GeoNamesDataStorage::getCountryList() const
    {
        return _countryLocationsMap.keys();
    }

    bool GeoNamesDataStorage::addData(const QPair<QString, QList<GeoNames::GeoParsingData>> &data)
    {
        const auto &[countryCode, newLocations] = data;

        if (newLocations.isEmpty())
        {
            return false;
        }

        auto& existingList = _countryLocationsMap[countryCode];
        const int oldSize = existingList.size();

        if(oldSize == 0)
        {
            existingList = newLocations;
            return true;
        }

        QSet<GeoParsingData> uniqueSet(existingList.begin(), existingList.end());
        uniqueSet.unite(QSet<GeoParsingData>(newLocations.begin(), newLocations.end()));

        if (uniqueSet.size() != oldSize)
        {
            existingList = uniqueSet.values();

            return true;
        }

        return false;
    }
}
