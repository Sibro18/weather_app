#ifndef I_GEO_NAMES_SERVICE_H
#define I_GEO_NAMES_SERVICE_H


#include "../../../core/entities/geo-names/location-data/location-data.h"
#include "../../../core/entities/db-requests/pagination-params.h"


namespace GeoNames 
{
    /**
     * @brief Interface for GeoNames geographical data services.
     */
    class IGeoNamesService 
    {
    public:
        /**
         * @brief Virtual destructor.
         */
        virtual ~IGeoNamesService() = default;

        /**
         * @brief Retrieve geographical locations for a specific country.
         * @param countryCode - ISO country code (e.g., "US", "DE", "FR").
         * @param pagination - Optional pagination parameters for large result sets.
         * @return Optional containing list of locations, empty if country not found.
         */
        virtual std::optional<QList<LocationData>> getLocationData(QString countryCode, std::optional<PaginationParams> pagination) = 0;

        /**
         * @brief Retrieve list of available country codes.
         * @param pagination - Optional pagination for limiting results.
         * @return List of ISO country codes.
         */
        virtual QList<QString> getCountryCodeList(std::optional<PaginationParams> pagination) = 0;

        /**
         * @brief Add or update location data for a country.
         * @param countryCode - ISO country code to add locations for.
         * @param locations - List of geographical locations to add.
         */
        virtual void addCountryLocations(QString countryCode, QList<LocationData> locations) = 0;
    };
}

#endif // I_GEO_NAMES_SERVICE_H
