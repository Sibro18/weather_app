#ifndef GEO_NAMES_DATA_SORAGE_H
#define GEO_NAMES_DATA_SORAGE_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QPair>

#include "../../data-structs/geo-names/geo-parsing-data/geo-parsing-data.h"

namespace GeoNames
{
    /**
     * @brief Storage for country data.
     */
    class GeoNamesDataStorage : public QObject
    {
        Q_OBJECT
    public:
        /**
         * @brief Create storage.
         * @param parent - parent object.
         */
        explicit GeoNamesDataStorage(QObject *parent = nullptr);

        /**
         * @brief Create storage.
         * @param countryList - init country list.
         * @param parent - parent object.
         */
        explicit GeoNamesDataStorage(const QStringList &countryList, QObject *parent = nullptr);

        /**
         * @brief Get geographical data for specified country.
         * @param countryCode - ISO country code.
         * @return Variant map containing country data.
         */
        QVariantMap getCountryData(const QString &countryCode) const;

        /**
         * @brief Get list of all available country codes.
         * @return String list of country codes.
         */
        QStringList getCountryList() const;

        /**
         * @brief Add geographical data for a country.
         * @param data - Pair containing country code and list of locations.
         * @return true if new data was added, false if data already exists.
         */
        bool addData(const QPair<QString, QList<GeoNames::GeoParsingData>> &data);
    private:
        /**
         * @brief In-memory storage for country geographical data.
         *
         * Key: ISO 3166 country code (e.g., "US", "DE", "FR");
         * Value: List of geographical locations within the country.
         */
        QMap<QString, QList<GeoNames::GeoParsingData>> _countryLocationsMap;
    };
}

#endif // GEO_NAMES_DATA_SORAGE_H
