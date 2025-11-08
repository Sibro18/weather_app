#include <QVariantList>
#include <QSet>

#include "geo-names-cache-service.h"


namespace GeoNames
{
    GeoNamesCacheService::GeoNamesCacheService(QObject *parent)
        : QObject{parent}
    {}

    GeoNamesCacheService::GeoNamesCacheService(const QStringList &countryList, QObject *parent)
        : QObject{parent}
    {
        for (const auto &item : countryList)
        {
            _countryLocationsMap[item] = {};
        }
    }

    std::optional<QList<LocationData>> GeoNamesCacheService::get(const QString& key)
    {
        auto it = _countryLocationsMap.constFind(key);

        if (it == _countryLocationsMap.constEnd())
        {
            return {};
        }

        return *it;
    }

    void GeoNamesCacheService::set(const QString& key, QList<LocationData> data)
    {
        if (data.isEmpty())
        {
            return;
        }

        auto& existingList = _countryLocationsMap[key];
        const int oldSize = existingList.size();

        if(oldSize == 0)
        {
            existingList = std::move(data);

            return;
        }

        QSet<LocationData> uniqueSet(existingList.begin(), existingList.end());
        uniqueSet.unite(QSet<LocationData>(data.begin(), data.end()));

        if (uniqueSet.size() != oldSize)
        {
            existingList = uniqueSet.values();

            return;
        }
    }

    void GeoNamesCacheService::remove(const QString& key)
    {
        _countryLocationsMap.remove(key);
    }

    void GeoNamesCacheService::clear()
    {
        _countryLocationsMap.clear();
    }

    bool GeoNamesCacheService::contains(const QString& key)
    {
        return _countryLocationsMap.contains(key);
    }

    QStringList GeoNamesCacheService::getCountryList() const
    {
        return _countryLocationsMap.keys();
    }

    void GeoNamesCacheService::setCountryList(QStringList countryList)
    {
        for (const auto& it : countryList)
        {
            _countryLocationsMap[it] = {};
        }
    }
}
