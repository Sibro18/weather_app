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
         * @param apiKey - GeoNames API username.
         * @param parent - Parent QObject.
         */
        explicit GeoNamesApiController(const QString &apiKey, GeneralUtils::TaskManager* taskManager, QObject* parent = nullptr);

        /**
         * @brief Fetches country data using the specified request parameters.
         * @param requestData - Request parameters.
         */
        void fetchCountryData(const GeoNames::RequestData &requestData);
    signals:
        /**
         * @brief Emitted when data for a country has been fetched.
         * @param fetchResult - Result of the fetch operation.
         */
        void dataByCountryFetched(GeoNames::FetchResult* fetchResult);
    private:
        /**
         * @brief GeoNames API username.
         */
        QString _apiKey;

        /**
         * @brief URL template for API requests.
         */
        const QString _apiUrlTemplate = "http://api.geonames.org/searchJSON?username=%1&country=%2";

        QNetworkAccessManager _manager;
        GeneralUtils::TaskManager* _taskManager;

        /**
         * @brief get builded request URL.
         * @param requestData - parameters for API request.
         * @return Fully constructed Url for GeoNames API call.
         */
        QUrl _buildRequestUrl(const RequestData &requestData) const;

        /**
         * @brief Handles ther network reply.
         * @param reply - API response.
         * @param requestData - Original request data.
         * @return result of fetching containing parsed data and error information.
         */
        FetchResult* _getFetchResult(const QByteArray &payload, const RequestData &requestData, const QString &errorString);


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
        QString _getUrlInfoByRequest(const RequestData &requestData) const;
    };
}

#endif // GEO_NAMES_DATA_CONTROLLER_H
