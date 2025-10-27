#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QPointer>
#include <QStringList>
#include <QThread>
#include "geo-names-api-controller.h"
#include "../../utils/backend-config/backend-config.h"

namespace GeoNames
{
    GeoNamesApiController::GeoNamesApiController(GeneralUtils::TaskManager* taskManager , QObject* parent)
        : QObject(parent), _taskManager(taskManager)
    {}

    void GeoNamesApiController::fetchCountryData(RequestData requestData)
    {
        if (requestData.countryCode.isEmpty()) {
            _log("Validation", "Country code is empty!");

            return;
        }

        QUrl url = _buildRequestUrl(requestData, "/searchJSON");

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

        connect(reply, &QNetworkReply::finished, this, [this, reply, requestData = std::move(requestData)]()
        {
            const auto errorString = reply->error() == QNetworkReply::NoError
             ? ""
             :reply->errorString();

            _taskManager->runAsync(GeneralUtils::Priority::High, [
                controller = QPointer<GeoNamesApiController>(this),
                payload = std::move(reply->readAll()),
                errorString = std::move(errorString),
                requestData = std::move(requestData)] ()
            {
                if (!controller)
                {
                    return;
                }

                FetchResult* fetchResult { new FetchResult() };

                fetchResult->data.first = requestData.countryCode;
                fetchResult->requestData = std::move(requestData);

                if (!errorString.isEmpty()) {
                    fetchResult->errorString = errorString;
                } else {
                    controller->_fillFetchResult(payload, fetchResult);
                }

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


    QUrl GeoNamesApiController::_buildRequestUrl(const RequestData &requestData, const QString &endpoint) const
    {
        const auto serviceConfig = GeneralUtils::BackendConfig::geoNames().api;

        QString url = QString(serviceConfig.url + endpoint + "?username=%1")
            .arg(serviceConfig.key);

        url += _getFilledUrlParamsByRequest(requestData);

        return QUrl(url);
    }

    void GeoNamesApiController::_fillFetchResult(const QByteArray &payload, FetchResult* fetchResult)
    {
        QJsonParseError jsonError;
        const QJsonDocument doc = QJsonDocument::fromJson(std::move(payload), &jsonError);

        if (jsonError.error != QJsonParseError::NoError)
        {
            qDebug() << jsonError.errorString();

            fetchResult->errorString = jsonError.errorString();
        }

        QString parseError;

        fetchResult->data.second = _parseResponse(doc, parseError);

        if (!parseError.isEmpty())
        {
            fetchResult->errorString = parseError;
        }
    }

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

    void GeoNamesApiController::_log(const QString& context, const QString& message) const
    {
        qWarning() << QString("[GeoNamesApiController].%1").arg(context) << "\t" << message;
    }

    QString GeoNamesApiController::_getFilledUrlParamsByRequest(const RequestData &requestData) const
    {
        QString urlInfo = "";
        QString featureClass = requestData.featureClass.has_value()
         ? requestData.featureClass.value().toString()
         : "";

        if (!requestData.countryCode.isEmpty())
        {
            urlInfo += QString("&country=%1").arg(requestData.countryCode);
        }

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
