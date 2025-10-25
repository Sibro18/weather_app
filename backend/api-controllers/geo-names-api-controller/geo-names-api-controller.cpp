#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QPointer>
#include <QStringList>
#include <QThread>
#include "geo-names-api-controller.h"


namespace GeoNames
{
    // WELL!!!
    GeoNamesApiController::GeoNamesApiController(const QString &apiKey, GeneralUtils::TaskManager* taskManager , QObject* parent)
        : QObject(parent), _apiKey(apiKey), _taskManager(taskManager)
    {}

    // WELL!!!
    void GeoNamesApiController::fetchCountryData(const RequestData &requestData)
    {
        QUrl url = _buildRequestUrl(requestData);

        QNetworkRequest request(url);

        _log(
            QString("_fetchDataByApi(countryCode: %1, name: %2, lang: %3, startWith: %4, featureClass: %5)")
                .arg(
                    requestData.countryCode,
                    requestData.name,
                    requestData.lang,
                    requestData.startWith
                     ? "true"
                     : "false",
                    requestData.featureClass.has_value()
                     ? requestData.featureClass.value().toString()
                     : "none"
                ),
            QString("URL: %0")
                .arg(url.toString())
        );

        QNetworkReply *reply = _manager.get(request);

        connect(reply, &QNetworkReply::finished, this, [this, reply, requestData]()
        {
            const auto errorString = reply->error() == QNetworkReply::NoError
             ? ""
             :reply->errorString();

            _taskManager->runAsync(GeneralUtils::Priority::High, [
                controller = QPointer<GeoNamesApiController>(this),
                payload = std::move(reply->readAll()),
                errorString = std::move(errorString),
                requestData] ()
            {
                if (!controller)
                {
                    return;
                }

                const auto fetchResult = controller->_getFetchResult(payload, requestData, errorString);

                QMetaObject::invokeMethod(controller, [controller, fetchResult = std::move(fetchResult)]() {
                    if (!controller)
                    {
                        return;
                    }

                    emit controller->dataByCountryFetched(fetchResult);
                }, Qt::QueuedConnection);
            });

            reply->deleteLater();
        });
    }

    // WELL!!!
    QUrl GeoNamesApiController::_buildRequestUrl(const RequestData &requestData) const
    {
        QString baseUrl = QString(_apiUrlTemplate)
            .arg(_apiKey, requestData.countryCode);

        baseUrl += _getUrlInfoByRequest(requestData);

        return QUrl(baseUrl);
    }

    // WELL!!!
    FetchResult* GeoNamesApiController::_getFetchResult(const QByteArray &payload, const RequestData &requestData, const QString &errorString)
    {
        FetchResult* fetchResult { new FetchResult() };

        fetchResult->requestData = requestData;
        fetchResult->data.first = requestData.countryCode;

        if (!errorString.isEmpty())
        {
            fetchResult->errorString = errorString;

            return fetchResult;
        }

        QJsonParseError jsonError;
        const QJsonDocument doc = QJsonDocument::fromJson(std::move(payload), &jsonError);

        if (jsonError.error != QJsonParseError::NoError)
        {
            qDebug() << jsonError.errorString();

            fetchResult->errorString = jsonError.errorString();

            return fetchResult;
        }

        QString parseError;

        fetchResult->data.second = _parseResponse(doc, parseError);

        if (!parseError.isEmpty())
        {
            fetchResult->errorString = parseError;
        }

        return fetchResult;
    }

    // WELL!!!
    QList<GeoParsingData> GeoNamesApiController::_parseResponse(const QJsonDocument& doc, QString& errorString) const
    {
        const auto invalidJsonError = "Invalid JSON response";

        if (doc.isNull() || doc.isEmpty())
        {
            errorString = invalidJsonError;
        }

        if (doc.isArray())
        {
            return GeoParsingData::fromJsonArray(doc.array());
        }

        auto root = doc.object();
        auto arrVal = root.find("geonames");

        if (!arrVal->isArray())
        {
            errorString = invalidJsonError;
        }

        return GeoParsingData::fromJsonArray(arrVal->toArray());
    }

    // WELL!!!
    void GeoNamesApiController::_log(const QString& context, const QString& message) const
    {
        qWarning() << QString("[GeoNamesApiController].%1").arg(context) << "\t" << message;
    }

    // WELL!!!
    QString GeoNamesApiController::_getUrlInfoByRequest(const RequestData &requestData) const
    {
        QString urlInfo = "";
        QString featureClass = requestData.featureClass.has_value()
         ? requestData.featureClass.value().toString()
         : "";

        if (!featureClass.isEmpty())
        {
            urlInfo += QString("&featureClass=%1").arg(featureClass);
        }

        if (!requestData.startWith && !requestData.name.isEmpty())
        {
            urlInfo += QString("&name_equals=%1").arg(QUrl::toPercentEncoding(requestData.name));
        }
        else if (requestData.startWith && !requestData.name.isEmpty())
        {
            urlInfo += QString("&name_startsWith=%1").arg(QUrl::toPercentEncoding(requestData.name));
        }

        if (!requestData.lang.isEmpty())
        {
            urlInfo += QString("&lang=%1").arg(requestData.lang);
        }

        return urlInfo;
    }
}
