#ifndef GEO_NAMES_MANAGER_H
#define GEO_NAMES_MANAGER_H


#include <QString>
#include <QtNetwork/QNetworkAccessManager>
#include <QList>
#include <QPair>
#include <QStringList>
#include <memory>

#include "../../api-controllers/geo-names-api-controller/geo-names-api-controller.h"
#include "../../utils/task-manager/task-manager.h"
#include "../../utils/file-service/file-service.h"


namespace GeoNames
{
    /**
     * @brief Bridge between the GeoNames API controller and the rest of the application.
     */
    class GeoNamesManager : public QObject
    {
        Q_OBJECT
    public:
        /**
         * @brief Constructs a GeoNamesDataBridge instance.
         * @param apiController - Pointer to the GeoNamesApiController responsible for API calls.
         * @param taskManager - Pointer to TaskManager for running asynchronous tasks.
         * @param fileService - Pointer to FileService for file I/O operations.
         * @param parent - Optional parent QObject.
         */
        explicit GeoNamesManager(
            GeoNames::GeoNamesApiController* apiController,
            GeneralUtils::TaskManager* taskManager,
            GeneralUtils::FileService* fileService,
            QObject* parent = nullptr
        );

        /**
         * @brief Get all available countries.
         */
        QStringList getCountryList() const;

        /**
         * @brief Fetch data from file system by countryCode.
         * @param countryCode - code of country.
         */
        void fetchDataFromFileSystem(const QString& countryCode);

        /**
         * @brief Fetches GeoNames data for the given request parameters.
         * @param requestData - Parameters specifying the request for GeoNames data.
         */
        void fetchDataByRequestAsync(GeoNames::RequestData requestData);

    signals:
        /**
         * @brief Emitted when GeoNames data has been fetched and parsed.
         * @param fetchResult - The result of the fetch operation.
         */
        void geoNamesFetched(GeoNames::FetchResult* fetchResult);
    private:
        /**
         * @brief Pointer to the task manager for executing asynchronous work.
         */
        GeneralUtils::TaskManager* _taskManager;

        /**
         * @brief Pointer to the file service for performing file system operations.
         */
        GeneralUtils::FileService* _fileService;

        /**
         * @brief Pointer to the API controller for making GeoNames requests.
         */
        GeoNames::GeoNamesApiController* _apiController;

        /**
         * @brief Template path for the cache directory where fetched data is stored.
         */
        const QString _cacheFilePathTemplate = "data/%1.json";

        /**
         * @brief dirPath.
         */
        const QString _cacheDirPath = "data";

        /**
         * @brief Attempts to read GeoNames data from the local file system.
         * @param filePath - Path to the cached data file.
         * @return A FetchResult if the file exists and is valid; std::nullopt otherwise.
         */
        std::unique_ptr<GeoNames::FetchResult> _fetchFromFileSystem(const QString &filePath);

        /**
         * @brief Saves the provided GeoNames data to the local file system cache.
         * @param fetchResult - The result containing GeoNames request info and parsed data.
         */
        void _saveDataToFileSystemAsync(const QPair<QString, QList<GeoParsingData>>& data);

        /**
         * @brief Get converted byte array to variant map.
         * @param jsonData - Byte array storing data in json view.
         * @return QVariantMap - parsed variant map from byte array.
         */
        QVariantMap _byteArrayToVariantMap(const QByteArray& jsonData);

        QVariantMap _serializeCountryData(const QPair<QString, QList<GeoParsingData>>& data) const;

        QList<GeoParsingData> _deserialaizeCountryData(QVariantMap data) const;
    private slots:
        /**
         * @brief Handles GeoNames data received from the API controller.
         * @param fetchResult - The result returned from the API call.
         */
        void handleDataFromApiFetched(GeoNames::FetchResult* fetchResult);
    };
}

#endif // GEO_NAMES_MANAGER_H
