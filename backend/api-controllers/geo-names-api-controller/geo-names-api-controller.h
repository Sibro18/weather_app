#ifndef GEO_NAMES_DATA_CONTROLLER_H
#define GEO_NAMES_DATA_CONTROLLER_H

#include <QString>
#include <QtNetwork/QNetworkAccessManager>
#include <QList>
#include <QPair>

#include "../../data-structs/geo-names/request-data/request-data.h"
#include "../../data-structs/geo-names/fetch-result/fetch-result.h"
#include "../../utils/task-manager/task-manager.h"

namespace GeoNames
{
    /**
     * @brief Controller for retrieving GeoNames data by country.
     */
    class GeoNamesApiController : public QObject
    {
        Q_OBJECT
    public:
        /**
         * @brief Constructs the controller with the given API key.
         * @param parent - Parent QObject.
         */
        explicit GeoNamesApiController(
            GeneralUtils::TaskManager* taskManager,
            QObject* parent = nullptr
        );

        /**
         * @brief Fetches country data using the specified request parameters.
         * @param requestData - Request parameters.
         */
        void fetchCountryData(GeoNames::RequestData requestData);
    signals:
        /**
         * @brief Emitted when data for a country has been fetched.
         * @param fetchResult - Result of the fetch operation.
         */
        void dataByCountryFetched(GeoNames::FetchResult* fetchResult);
    private:
        /**
         * @brief Manager for network requests.
         */
        QNetworkAccessManager _manager;

        /**
         * @brief Manager for async operations.
         */
        GeneralUtils::TaskManager* _taskManager;

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
        void _fillFetchResult(const QByteArray &payload, FetchResult* fetchResult);

        /**
         * @brief Parses JSON response from GeoNames API.
         * @param doc - JSON document containing API response.
         * @param errorString - Reference to string where error message will be stored if parsing fails.
         * @return List of parsed geographical data objects.
         */
        QList<GeoParsingData> _parseResponse(const QJsonDocument& doc, QString& errorString) const;

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

#endif // GEO_NAMES_DATA_CONTROLLER_H
