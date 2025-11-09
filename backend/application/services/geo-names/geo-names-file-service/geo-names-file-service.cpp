#include <QFileInfo>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardPaths>

#include "geo-names-file-service.h"


namespace GeoNames
{
    GeoNamesFileService::GeoNamesFileService(Common::FileService* fileService, QPair<QString, bool> dirFilePathData, QObject* parent)
        :
        QObject(parent),
        _fileService(fileService)
    {
        const auto& [dirFilePath, isAbsolute] = dirFilePathData;

        if (!isAbsolute)
        {
            QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
            _dirFilePath = dataPath + dirFilePath;
        }
        else
        {
            _dirFilePath = dirFilePath;
        }

        _fileService->createNewDirIfNotExists(_dirFilePath);
    }

    std::optional<QList<LocationData>> GeoNamesFileService::getLocationData(QString countryCode, std::optional<PaginationParams> pagination)
    {
        const auto filePath = QString(_dirFilePath + "/%1.json").arg(countryCode);
        auto data = _fileService->getData(filePath);

        if (!data)
        {
            return {};
        }

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(*data, &parseError);

        if (doc.isObject())
        {
            QJsonObject dataInJson = doc.object();

            if (dataInJson.contains("items"))
            {
                QJsonObject itemsObj = dataInJson["items"].toObject();
                QJsonArray jsonArray;

                for (auto it = itemsObj.begin(); it != itemsObj.end(); ++it)
                {
                    jsonArray.append(it.value());
                }

                return LocationData::fromJsonArray(jsonArray);
            }
        }

        return {};
    }

    QList<QString> GeoNamesFileService::getCountryCodeList(std::optional<PaginationParams> pagination)
    {
        const auto fileNameListOpt = _fileService->getFileNamesByDir(
            _dirFilePath
        );

        if (!fileNameListOpt)
        {
            return {};
        }

        QStringList countryList;
        countryList.reserve(fileNameListOpt->size());

        for (const auto &fileName : *fileNameListOpt)
        {
            if (fileName.endsWith(".json", Qt::CaseInsensitive))
            {
                QFileInfo fileInfo(fileName);
                countryList.append(fileInfo.baseName());
            }
        }

        return countryList;
    }

    void GeoNamesFileService::addCountryLocations(QString countryCode, QList<LocationData> newLocations)
    {
        const auto filePath = QString(_dirFilePath + "/%1.json").arg(countryCode);
        const auto existingData = _fileService->getData(filePath);

        QList<LocationData> mergedData;

        if (existingData)
        {
            auto existingLocations = _deserialaizeCountryData(
                _byteArrayToVariantMap(*existingData)
            );

            QSet<LocationData> uniqueSet(existingLocations.begin(), existingLocations.end());
            uniqueSet.unite(QSet<LocationData>(newLocations.begin(), newLocations.end()));

            mergedData = uniqueSet.values();
        }
        else
        {
            mergedData = newLocations;
        }

        QJsonDocument savedDoc(
            QJsonObject::fromVariantMap(
                _serializeCountryData(
                    qMakePair(countryCode, std::move(mergedData))
                )
            )
        );

        QMap<QString, QByteArray> saveMap;
        saveMap[filePath] = savedDoc.toJson(QJsonDocument::Compact);

        _fileService->saveData(saveMap);
    }

    QVariantMap GeoNamesFileService::_byteArrayToVariantMap(const QByteArray& jsonData)
    {
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);

        if (parseError.error != QJsonParseError::NoError)
        {
            return QVariantMap();
        }

        if (!doc.isObject())
        {
            return QVariantMap();
        }

        return doc.object().toVariantMap();
    }

    QVariantMap GeoNamesFileService::_serializeCountryData(const QPair<QString, QList<LocationData>>& data) const
    {
        QVariantMap returnData;

        returnData["name"] = data.first;
        returnData["type"] = "country";

        QVariantMap itemData;

        for (const auto& geoData : data.second)
        {
            auto&& key = QString("%1_%2").arg(geoData.latitude, geoData.longitude);
            itemData[key] = geoData.toVariantMap();
        }

        returnData["items"] = itemData;

        return returnData;
    }

    QList<LocationData> GeoNamesFileService::_deserialaizeCountryData(QVariantMap data) const
    {
        const auto itemsMap = data["items"].toMap();

        QList<LocationData> returnData;
        returnData.reserve(itemsMap.size());

        for (const auto& [key, value] : itemsMap.asKeyValueRange())
        {
            returnData.append(
                LocationData::convertToGeoParsingData(value.toMap())
            );
        }

        return returnData;
    }
}
