#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QPointer>


#include "geo-names-api-controller.h"


namespace GeoNames
{
    GeoNamesApiController::GeoNamesApiController(GeneralUtils::TaskManager* taskManager , GeneralUtils::BackendConfig::ApiConfig apiConfig, QObject* parent)
        : GeoNames::ILocationsApiController(parent), _taskManager(taskManager), _apiConfig(apiConfig)
    {}

    void GeoNamesApiController::fetchData(RequestData requestData)
    {
        const QString errorTemplate = "GeoNamesApiController::fetchData => %1";

        if (requestData.countryCode.isEmpty())
        {
            emit errorOccurred(
                errorTemplate.arg("Country code is empty!")
            );

            return;
        }

        QUrl url = _buildRequestUrl(requestData, "/searchJSON");
        QNetworkRequest request(url);

        QNetworkReply *reply = _manager.get(request);

        connect(reply, &QNetworkReply::finished, this, [this, reply, requestData = std::move(requestData), errorTemplate = std::move(errorTemplate)]()
        {
            QString errorString = reply->error() == QNetworkReply::NoError
             ? ""
             :reply->errorString();

            if (!errorString.isEmpty()) {
                QMetaObject::invokeMethod(this, [this, error = errorTemplate.arg(errorString)]() {
                    emit errorOccurred(error);
                }, Qt::QueuedConnection);

                return;
            }

            _taskManager->runAsync(GeneralUtils::Priority::High, [
                controller = QPointer<GeoNamesApiController>(this),
                payload = std::move(reply->readAll()),
                requestData = std::move(requestData),
                errorTemplate = std::move(errorTemplate)
            ] ()
            {
                if (!controller)
                {
                    return;
                }

                FetchResult fetchResult;

                fetchResult.data.first = requestData.countryCode;
                fetchResult.requestData = std::move(requestData);

                QString error = "";
                controller->_handleRequestPayload(payload, fetchResult, error);

                QMetaObject::invokeMethod(controller, [controller, fetchResult = std::move(fetchResult), error = std::move(error)]() {
                    if (!controller)
                    {
                        return;
                    }

                    error.isEmpty()
                        ? emit controller->dataFetched(fetchResult)
                        : emit controller->errorOccurred(error);
                }, Qt::QueuedConnection);
            });

            reply->deleteLater();
        });
    }


    QUrl GeoNamesApiController::_buildRequestUrl(const RequestData &requestData, const QString &endpoint) const
    {
        QString url = QString(_apiConfig.url + endpoint + "?username=%1")
            .arg(_apiConfig.key);

        url += _getFilledUrlParamsByRequest(requestData);

        return QUrl(url);
    }

    void GeoNamesApiController::_handleRequestPayload(const QByteArray &payload, FetchResult &fetchResult, QString &errorString)
    {
        QJsonParseError jsonError;
        const QJsonDocument doc = QJsonDocument::fromJson(std::move(payload), &jsonError);

        if (jsonError.error != QJsonParseError::NoError)
        {
            qDebug() << jsonError.errorString();

            errorString = jsonError.errorString();
        }

        QString parseError;

        fetchResult.data.second = _parseResponse(doc, parseError);

        if (!parseError.isEmpty())
        {
            errorString = parseError;
        }
    }

    QList<LocationData> GeoNamesApiController::_parseResponse(const QJsonDocument& doc, QString& errorString) const
    {
        const auto invalidJsonError = "Invalid JSON response";

        if (doc.isNull() || doc.isEmpty())
        {
            errorString = invalidJsonError;
        }

        if (doc.isArray())
        {
            return LocationData::fromJsonArray(doc.array());
        }

        auto root = doc.object();
        auto arrVal = root.find("geonames");

        if (!arrVal->isArray())
        {
            errorString = invalidJsonError;
        }

        return LocationData::fromJsonArray(arrVal->toArray());
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
