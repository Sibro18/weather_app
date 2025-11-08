#ifndef I_GEO_NAMES_CACHE_SERVICE_H
#define I_GEO_NAMES_CACHE_SERVICE_H


#include "../../../core/entities/geo-names/location-data/location-data.h"
#include "../../../core/interfaces/i-cache-service.h"


namespace GeoNames
{
    /**
     * @brief Extended cache service interface for GeoNames location data.
     *
     * @tparam QList<GeoNames::LocationData> - List of geographical locations.
     */
    class IGeoNamesCacheService : public ICacheService<QList<GeoNames::LocationData>>
    {
    public:
        /**
         * @brief Virtual destructor.
         */
        virtual ~IGeoNamesCacheService() = default;

        /**
         * @brief Retrieve cached list of country names.
         * @return List of country names, empty if not cached.
         */
        virtual QStringList getCountryList() const = 0;

        /**
         * @brief Cache list of country names.
         * @param countryList - List of country names to cache.
         */
        virtual void setCountryList(QStringList countryList) = 0;
    };
}

#endif // I_GEO_NAMES_CACHE_SERVICE_H
