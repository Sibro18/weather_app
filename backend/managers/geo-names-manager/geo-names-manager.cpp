#include <QSet>
#include <QFileInfo>
#include <QPointer>
#include <QList>

#include "geo-names-manager.h"

namespace GeoNames
{
    GeoNamesManager::GeoNamesManager(
        GeoNamesApiController* apiController,
        GeneralUtils::TaskManager* taskManager,
        GeneralUtils::FileService* fileService,
        QObject* parent
    ) : QObject(parent),
        _apiController(apiController),
        _taskManager(taskManager),
        _fileService(fileService)
    {
        connect(
            _apiController,
            &GeoNames::GeoNamesApiController::dataByCountryFetched,
            this,
            &GeoNames::GeoNamesManager::handleDataFromApiFetched
        );
    }
    void GeoNamesManager::fetchDataByRequestAsync(GeoNames::RequestData requestData)
    {
        _apiController->fetchCountryData(std::move(requestData));
    }

    void GeoNamesManager::fetchDataFromFileSystem(const QString& countryCode)
    {
        _taskManager->runAsync(
            GeneralUtils::Priority::High,
            [
                controller = QPointer<GeoNamesManager>(this),
                countryCode,
                cachePath = this->_cacheFilePathTemplate.arg(countryCode)
            ] ()
            {
                auto data = controller->_fileService->getData(cachePath);

                if (!data)
                {
                    return;
                }

                QJsonParseError parseError;
                QJsonDocument doc = QJsonDocument::fromJson(*data, &parseError);

                GeoNames::FetchResult* fetchResult  = new FetchResult();

                fetchResult->data.first = countryCode;

                if (parseError.error != QJsonParseError::NoError)
                {
                    fetchResult->errorString = parseError.errorString();
                }
                else if (doc.isObject())
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

                        fetchResult->data.second = GeoParsingData::fromJsonArray(jsonArray);
                    }
                }

                QMetaObject::invokeMethod(controller, [controller, fetchResult]() {
                    if (controller)
                    {
                        emit controller->geoNamesFetched(fetchResult);
                    }
                }, Qt::QueuedConnection);
            }
        );
    }

    QStringList GeoNamesManager::getCountryList() const
    {
        const auto fileNameListOpt = _fileService->getFileNamesByDir(_cacheDirPath);

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


    void GeoNamesManager::handleDataFromApiFetched(GeoNames::FetchResult* fetchResult)
    {
        if (fetchResult->errorString.isEmpty())
        {
            _saveDataToFileSystemAsync(fetchResult->data);
        }

        emit geoNamesFetched(fetchResult);
    }

    void GeoNamesManager::_saveDataToFileSystemAsync(const QPair<QString, QList<GeoParsingData>>& data)
    {
        _taskManager->runAsync(
            GeneralUtils::Priority::Normal,
            [
                controller = QPointer<GeoNamesManager>(this),
                countryCode = data.first,
                newLocations = data.second,
                cacheFilePath = this->_cacheFilePathTemplate.arg(data.first)
            ] ()
            {
                if (!controller)
                {
                    return;
                }

                const auto existingData = controller->_fileService->getData(cacheFilePath);

                QList<GeoParsingData> mergedData;

                if (existingData != std::nullopt)
                {
                    auto existingLocations = controller->_deserialaizeCountryData(
                        controller->_byteArrayToVariantMap(*existingData)
                    );

                    QSet<GeoParsingData> uniqueSet(existingLocations.begin(), existingLocations.end());
                    uniqueSet.unite(QSet<GeoParsingData>(newLocations.begin(), newLocations.end()));

                    mergedData = uniqueSet.values();
                }
                else
                {
                    mergedData = newLocations;
                }

                QJsonDocument savedDoc(
                    QJsonObject::fromVariantMap(controller->_serializeCountryData(
                            qMakePair(countryCode, std::move(mergedData))
                        )
                    )
                );

                QMap<QString, QByteArray> saveMap;
                saveMap[cacheFilePath] = savedDoc.toJson(QJsonDocument::Compact);

                controller->_fileService->saveData(saveMap);
            }
        );
    }

    QVariantMap GeoNamesManager::_byteArrayToVariantMap(const QByteArray& jsonData)
    {
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);

        if (parseError.error != QJsonParseError::NoError)
        {
            qWarning() << "JSON parse error:" << parseError.errorString();

            return QVariantMap();
        }

        if (!doc.isObject())
        {
            qWarning() << "JSON is not an object";

            return QVariantMap();
        }

        return doc.object().toVariantMap();
    }

    QVariantMap GeoNamesManager::_serializeCountryData(const QPair<QString, QList<GeoParsingData>>& data) const
    {
        QVariantMap returnData;

        returnData["name"] = data.first;
        returnData["type"] = "country";

        QVariantMap itemData;

        for (const auto& geoData : data.second)
        {
            auto&& key = geoData.latitude + "_" + geoData.longitude;
            itemData[key] = geoData.toVariantMap();
        }

        returnData["items"] = itemData;

        return returnData;
    }

    QList<GeoParsingData> GeoNamesManager::_deserialaizeCountryData(QVariantMap data) const
    {
        const auto itemsMap = data["items"].toMap();

        QList<GeoParsingData> returnData;
        returnData.reserve(itemsMap.size());

        for (const auto& [key, value] : itemsMap.asKeyValueRange())
        {
            returnData.append(
                GeoParsingData::convertToGeoParsingData(value.toMap())
            );
        }

        return returnData;
    }
}
    
