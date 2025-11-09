#ifndef GEO_NAMES_FILE_SERVICE_H
#define GEO_NAMES_FILE_SERVICE_H


#include "../i-geo-names-service.h"
#include "../../../../utils/file-service/file-service.h"


namespace GeoNames 
{
    /**
     * @brief File-based implementation of GeoNames service using JSON storage.
     */
    class GeoNamesFileService : public QObject, public IGeoNamesService
    {
        Q_OBJECT
    public:
        /**
         * @brief Constructs file-based GeoNames service.
         * @param fileService - File service utility for file operations.
         * @param dirFilePathData -
         *      first: Directory path for storing country data files.
         *      second: isAbsolute. for Absoule paths.
         * @param parent - Parent object.
         */
        GeoNamesFileService(Common::FileService* fileService, QPair<QString, bool> dirFilePathData, QObject* parent = nullptr);

        /**
         * @brief Retrieve geographical locations for a specific country from file storage.
         * @param countryCode - ISO country code (e.g., "US", "DE", "FR").
         * @param pagination - Optional pagination parameters for large result sets.
         * @return Optional containing list of locations, empty if country file not found.
         */
        std::optional<QList<LocationData>> getLocationData(QString countryCode, std::optional<PaginationParams> pagination) override;

        /**
         * @brief Retrieve list of available country codes from directory structure.
         * @param pagination - Optional pagination for limiting results.
         * @return List of ISO country codes found in file storage.
         */
        QList<QString> getCountryCodeList(std::optional<PaginationParams> pagination) override;

        /**
         * @brief Add or update location data for a country in file storage.
         * @param countryCode - ISO country code to add locations for.
         * @param newLocations - List of geographical locations to add to storage.
         */
        void addCountryLocations(QString countryCode, QList<LocationData> newLocations) override;
    private:
        Common::FileService* _fileService; ///< File operations utility
        QString _dirFilePath;                    ///< Directory path for country data files

        /**
         * @brief Convert JSON byte array to variant map.
         * @param jsonData - Byte array containing JSON-formatted data.
         * @return Parsed QVariantMap from JSON data.
         */
        QVariantMap _byteArrayToVariantMap(const QByteArray& jsonData);

        /**
         * @brief Serialize country data to variant map for JSON storage.
         * @param data - Country code and location data pair to serialize.
         * @return QVariantMap suitable for JSON serialization.
         */
        QVariantMap _serializeCountryData(const QPair<QString, QList<LocationData>>& data) const;

        /**
         * @brief Deserialize country data from variant map.
         * @param data - Variant map from JSON file.
         * @return List of LocationData objects.
         */
        QList<LocationData> _deserialaizeCountryData(QVariantMap data) const;
    };
}

#endif // GEO_NAMES_FILE_SERVICE.H
