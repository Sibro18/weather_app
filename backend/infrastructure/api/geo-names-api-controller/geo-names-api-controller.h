#ifndef GEO_NAMES_API_CONTROLLER_H
#define GEO_NAMES_API_CONTROLLER_H


#include <QtNetwork/QNetworkAccessManager>

#include "../../../utils/backend-config/backend-config.h"
#include "../../../utils/task-manager/task-manager.h"
#include "../../../core/entities/geo-names/request-data/request-data.h"
#include "../../../core/entities/geo-names/fetch-result/fetch-result.h"
#include "../interfaces/i-locations-api-controller.h"


namespace GeoNames
{
    /**
     * @brief Controller for retrieving GeoNames data by country.
     */
    class GeoNamesApiController : public GeoNames::ILocationsApiController
    {
        Q_OBJECT
    public:
        /**
         * @brief Constructs the controller with the given API key.
         * @param parent - Parent QObject.
         */
        explicit GeoNamesApiController(
            Common::TaskManager* taskManager,
            Common::BackendConfig::ApiConfig apiConfig,
            QObject* parent = nullptr
        );

        /**
         * @brief Fetches country data using the specified request parameters.
         * @param requestData - Request parameters.
         */
        void fetchData(GeoNames::RequestData requestData) override;
    private:
        QNetworkAccessManager _manager; ///< Manager for network requests.
        Common::TaskManager* _taskManager; ///< Network manager for handling HTTP requests.
        Common::BackendConfig::ApiConfig _apiConfig; ///< Config for API.

        /**
         * @brief get builded request URL.
         * @param requestData - parameters for API request.
         * @return Fully constructed Url for GeoNames API call.
         */
        QUrl _buildRequestUrl(const RequestData &requestData, const QString &endpoint) const;

        /**
         * @brief Handles ther network reply and fill result of fetch.
         * @param reply - API response.
         * @param requestData - Original request data.
         */
        void _handleRequestPayload(const QByteArray &payload, FetchResult &fetchResult, QString &errorString);

        /**
         * @brief Parses JSON response from GeoNames API.
         * @param doc - JSON document containing API response.
         * @param errorString - Reference to string where error message will be stored if parsing fails.
         * @return List of parsed geographical data objects.
         */
        QList<LocationData> _parseResponse(const QJsonDocument& doc, QString& errorString) const;

        /**
         * @brief Logs a message with context.
         * @param context - Context of the log (e.g., method name and params).
         * @param message - Message to log.
         */
        void _log(const QString& context, const QString& message) const;

        /**
         * @brief Builds URL parameters based on request data.
         * @param requestData - Request parameters.
         * @return URL query string.
         */
        QString _getFilledUrlParamsByRequest(const RequestData &requestData) const;
    };
}

#endif // GEO_NAMES_API_CONTROLLER_H
