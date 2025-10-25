// #include <QSet>
// #include <QFileInfo>
// #include <QPointer>

// #include "geo-names-data-bridge.h"

// namespace GeoNames
// {
//     GeoNamesDataBridge::GeoNamesDataBridge(
//         GeoNamesApiController* apiController,
//         GeneralUtils::TaskManager* taskManager,
//         GeneralUtils::FileService* fileService,
//         QObject* parent
//     ) : QObject(parent),
//         _apiController(apiController),
//         _taskManager(taskManager),
//         _fileService(fileService)
//     {
//         connect(
//             _apiController,
//             &GeoNames::GeoNamesApiController::dataByCountryFetched,
//             this,
//             &GeoNames::GeoNamesDataBridge::handleDataFromApiFetched
//         );
//     }
//     void GeoNamesDataBridge::fetchDataByRequestAsync(const GeoNames::RequestData &requestData)
//     {
//         _apiController->fetchCountryData(requestData);
//     }

//     void GeoNamesDataBridge::fetchDataFromFileSystem(const QString& countryCode)
//     {
//         _taskManager->runAsync(
//             GeneralUtils::Priority::High,
//             [controller = QPointer<GeoNamesDataBridge>(this), countryCode = std::move(countryCode)] ()
//             {
//                 auto data = controller->_fileService->getData(
//                     controller->_cacheFilePathTemplate.arg(countryCode)
//                 );

//                 if (!data.has_value())
//                 {
//                     return;
//                 }

//                 QJsonParseError parseError;
//                 QJsonDocument doc = QJsonDocument::fromJson(data.value(), &parseError);

//                 GeoNames::FetchResult* fetchResult { new FetchResult() };

//                 fetchResult->data.first = countryCode;

//                 if (parseError.error != QJsonParseError::NoError)
//                 {
//                     fetchResult->errorString = parseError.errorString();
//                 }
//                 else
//                 {
//                     QJsonObject dataInJson;

//                     if (doc.isObject())
//                     {
//                         dataInJson = doc.object();

//                         if (dataInJson.contains("items"))
//                         {
//                             const auto jsonArray = QJsonArray::fromVariantList(
//                                 dataInJson["items"].toObject().toVariantHash().values()
//                             );

//                             fetchResult->data.second = GeoParsingData::fromJsonArray(jsonArray);
//                         }
//                     }
//                 }

//                 QMetaObject::invokeMethod(controller, [controller, fetchResult = std::move(fetchResult)]() {
//                     if (controller)
//                     {
//                         emit controller->geoNamesFetched(fetchResult);
//                     }
//                 }, Qt::QueuedConnection);
//             }
//         );
//     }

//     QStringList GeoNamesDataBridge::getCountryList() const
//     {
//         const auto fileNameListOpt = _fileService->getFileNamesByDir(_cacheDirPath);

//         if (!fileNameListOpt.has_value())
//         {
//             return {};
//         }

//         QStringList countryList;

//         for (const auto &fileName : fileNameListOpt.value())
//         {
//             QFileInfo fileInfo(fileName);

//             if (fileInfo.suffix().toLower() != "json")
//             {
//                 continue;
//             }

//             countryList.append(
//                 fileInfo.baseName()
//             );
//         }

//         return countryList;
//     }

//     void GeoNamesDataBridge::_saveDataToFileSystemAsync(const QPair<QString, QList<GeoParsingData>>& data)
//     {
//         _taskManager->runAsync(
//             GeneralUtils::Priority::Normal,
//             [controller = QPointer<GeoNamesDataBridge>(this), data = std::move(data)] ()
//             {
//                 if (!controller)
//                 {
//                     return;
//                 }

//                 const auto& countryCode = data.first;
//                 const auto& locationList = data.second;
//                 const auto cacheFilePath = controller->_cacheFilePathTemplate.arg(countryCode);
//                 const auto dataFromFyleSystem = controller->_fileService->getData(cacheFilePath);

//                 QList<GeoParsingData> savedData;

//                 if (dataFromFyleSystem != std::nullopt)
//                 {
//                     const auto variantMapFromFyleSystem = controller->_byteArrayToVariantMap(dataFromFyleSystem.value());

//                     QSet<GeoParsingData> uniqueSet;
//                     const auto oldData = controller->_deserialaizeCountryData(variantMapFromFyleSystem);

//                     uniqueSet.unite(QSet<GeoParsingData>{locationList.constBegin(), locationList.constEnd()});
//                     uniqueSet.unite(QSet<GeoParsingData>{oldData.constBegin(), oldData.constEnd()});

//                     savedData = std::move(uniqueSet.values());
//                 }
//                 else
//                 {
//                     savedData = locationList;
//                 }

//                 QJsonDocument savedDoc(
//                     QJsonObject::fromVariantMap(controller->_serializeCountryData(
//                             qMakePair(countryCode, std::move(savedData))
//                         )
//                     )
//                 );

//                 QMap<QString, QByteArray> saveMap;
//                 saveMap[cacheFilePath] = savedDoc.toJson(QJsonDocument::Compact);

//                 controller->_fileService->saveData(saveMap);
//             }
//         );
//     }

//     QVariantMap GeoNamesDataBridge::_byteArrayToVariantMap(const QByteArray& jsonData)
//     {
//         QJsonParseError parseError;
//         QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);

//         if (parseError.error != QJsonParseError::NoError)
//         {
//             qWarning() << "JSON parse error:" << parseError.errorString();

//             return QVariantMap();
//         }

//         if (!doc.isObject())
//         {
//             qWarning() << "JSON is not an object";

//             return QVariantMap();
//         }

//         return doc.object().toVariantMap();
//     }

//     void GeoNamesDataBridge::handleDataFromApiFetched(GeoNames::FetchResult* fetchResult)
//     {
//         if (fetchResult->errorString.isEmpty())
//         {
//             _saveDataToFileSystemAsync(fetchResult->data);
//         }

//         emit geoNamesFetched(fetchResult);
//     }

//     QVariantMap GeoNamesDataBridge::_serializeCountryData(const QPair<QString, QList<GeoParsingData>>& data) const
//     {
//         QVariantMap returnData;

//         returnData["name"] = data.first;
//         returnData["type"] = "country";

//         QVariantMap itemData;

//         for (const auto& geoData : data.second)
//         {
//             auto&& key = geoData.latitude + "_" + geoData.longitude;
//             itemData[std::move(key)] = geoData.toVariantMap();
//         }

//         returnData["items"] = itemData;

//         return returnData;
//     }

//     QList<GeoParsingData> GeoNamesDataBridge::_deserialaizeCountryData(QVariantMap data) const
//     {
//         QList<GeoParsingData> returnData;

//         const auto itemsMap = data["items"].toMap();

//         for (const auto& [key, value] : itemsMap.asKeyValueRange())
//         {
//             returnData.append(
//                 GeoParsingData::convertToGeoParsingData(value.toMap())
//             );
//         }

//         return returnData;
//     }
// }
    
