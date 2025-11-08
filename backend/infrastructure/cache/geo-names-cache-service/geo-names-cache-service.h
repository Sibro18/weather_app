#ifndef GEO_NAMES_CACHE_SERVICE_H
#define GEO_NAMES_CACHE_SERVICE_H


#include "../interfaces/i-geo-names-cache-service.h"


namespace GeoNames
{
    /**
     * @brief In-memory storage for country data implementing cache service interface.
     */
    class GeoNamesCacheService : public QObject, public IGeoNamesCacheService
    {
        Q_OBJECT
    public:
        /**
         * @brief Create storage.
         * @param parent - parent object.
         */
        explicit GeoNamesCacheService(QObject *parent = nullptr);

        /**
         * @brief Create storage.
         * @param countryList - init country list.
         * @param parent - parent object.
         */
        explicit GeoNamesCacheService(const QStringList &countryList, QObject *parent = nullptr);

        /**
         * @brief Retrieve location data for specific country.
         * @param key - ISO country code (e.g., "US", "DE").
         * @return Optional containing location list if found.
         */
        std::optional<QList<LocationData>> get(const QString& key) override;

        /**
         * @brief Store location data for specific country.
         * @param key - ISO country code (e.g., "US", "DE").
         * @param data - List of geographical locations to cache.
         */
        void set(const QString& key, QList<LocationData> data) override;

        /**
         * @brief Remove location data for specific country.
         * @param key - ISO country code to remove from cache.
         */
        void remove(const QString& key) override;

        /**
         * @brief Clear all cached country data and country list.
         */
        void clear() override;

        /**
         * @brief Check if country data exists in cache.
         * @param key - ISO country code to check.
         * @return True if country data is cached.
         */
        bool contains(const QString& key) override;

        /**
         * @brief Get list of all available country codes.
         * @return String list of country codes.
         */
        QStringList getCountryList() const override;

        /**
         * @brief Cache list of country names/codes.
         * @param countryList - List of country identifiers to store.
         */
        void setCountryList(QStringList countryList) override;
    private:
        /**
         * @brief In-memory storage for country geographical data.
         *
         * Key: ISO 3166 country code (e.g., "US", "DE", "FR");
         * Value: List of geographical locations within the country.
         */
        QMap<QString, QList<GeoNames::LocationData>> _countryLocationsMap;
    };
}

#endif // GEO_NAMES_CACHE_SERVICE_H
