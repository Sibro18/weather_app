#ifndef I_LOCATIONS_API_CONTROLLER_H
#define I_LOCATIONS_API_CONTROLLER_H


#include "../../../core/interfaces/i-api-controller.h"
#include "../../../core/entities/geo-names/request-data/request-data.h"
#include "../../../core/entities/geo-names/fetch-result/fetch-result.h"


namespace GeoNames
{
    /**
     * @brief Interface for locations API controllers handling locations data requests.
     */
    class ILocationsApiController : public QObject, public IApiController<GeoNames::RequestData>
    {
        Q_OBJECT
    public:
        /**
         * @brief Create locationsApiController.
         * @param parent - parent object.
         */
        inline explicit ILocationsApiController(QObject *parent = nullptr)
            : QObject{parent}
        {}
    signals:
        /**
         * @brief Emitted when location data has been successfully fetched.
         * @param response - Fetch result containing location data.
         */
        void dataFetched(const GeoNames::FetchResult &response);

        /**
         * @brief Emitted when an error occurs during API request.
         * @param error - Human-readable error description.
         */
        void errorOccurred(QString error);
    };
}

#endif // I_LOCATIONS_API_CONTROLLER_H
