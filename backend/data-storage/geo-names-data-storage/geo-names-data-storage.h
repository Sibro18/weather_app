#ifndef GEO_NAMES_DATA_SORAGE_H
#define GEO_NAMES_DATA_SORAGE_H

#include <QObject>
#include <QString>
#include <QVariantList>
#include <QMap>
#include <QPair>
#include <QSet>

#include "../../data-structs/geo-names/geo-parsing-data/geo-parsing-data.h"

namespace GeoNames
{
    class GeoNamesDataStorage : public QObject
    {
        Q_OBJECT
    public:
        explicit GeoNamesDataStorage(QObject *parent = nullptr);
        explicit GeoNamesDataStorage(QStringList countryList, QObject *parent = nullptr);

        QVariantMap getCountryData(const QString &countryCode) const;
        QStringList getCountryList() const;

        bool addData(const QPair<QString, QList<GeoNames::GeoParsingData>> &data);
    signals:

    private:
        QMap<QString, QList<GeoNames::GeoParsingData>> _countryLocationsMap;
    };

}

#endif // GEO_NAMES_DATA_SORAGE_H
