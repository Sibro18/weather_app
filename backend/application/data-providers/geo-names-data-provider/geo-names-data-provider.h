#ifndef GEO_NAMES_DATA_PROVIDER_H
#define GEO_NAMES_DATA_PROVIDER_H


#include "../../../infrastructure/api/interfaces/i-locations-api-controller.h"
#include "../../../infrastructure/cache/interfaces/i-geo-names-cache-service.h"
#include "../../services/geo-names/i-geo-names-service.h"
#include "../../../utils/task-manager/task-manager.h"


namespace GeoNames
{
    /**
     * @brief Coordinates GeoNames data flow between API, cache, and services.
     */
    class GeoNamesDataProvider : public QObject
    {
        Q_OBJECT
    public:
        /**
         * @brief Constructs GeoNames data provider.
         * @param apiController - GeoNames API controller for data fetching.
         * @param taskManager - Task manager for asynchronous operations.
         * @param geoNamesCacheService - Cache service for GeoNames data.
         * @param geoNamesService - GeoNames data service.
         * @param parent - Optional parent QObject.
         */
        explicit GeoNamesDataProvider(
            GeoNames::ILocationsApiController* apiController,
            GeneralUtils::TaskManager* taskManager,
            GeoNames::IGeoNamesCacheService* geoNamesCacheService,
            GeoNames::IGeoNamesService* geoNamesService,
            QObject* parent = nullptr
        );

        /**
         * @brief Get available country list.
         * @return List of country names/codes.
         */
        QStringList getCountryList() const;

        /**
         * @brief Fetch GeoNames data asynchronously.
         * @param requestData - Parameters for GeoNames data request.
         */
        void fetchDataByRequestAsync(GeoNames::RequestData requestData);
    signals:
        /**
         * @brief GeoNames data fetched and ready.
         * @param fetchResult - Result of GeoNames fetch operation.
         */
        void geoNamesFetched(const GeoNames::FetchResult &fetchResult);
    private:
        GeneralUtils::TaskManager* _taskManager;                ///< Task manager for asynchronous operations.
        GeoNames::ILocationsApiController* _apiController;      ///< GeoNames API controller for data fetching.
        GeoNames::IGeoNamesCacheService* _geoNamesCacheService; ///< Cache service for GeoNames data.
        GeoNames::IGeoNamesService* _geoNamesService;           ///< GeoNames data service.
    private slots:
        /**
         * @brief Process API response data.
         * @param fetchResult - API response data to process.
         */
        void handleDataFromApiFetched(const GeoNames::FetchResult &fetchResult);
    };
}

#endif // GEO_NAMES_DATA_PROVIDER_H
