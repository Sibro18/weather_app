#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>
#include <QPointer>
#include <QJsonArray>
#include <QTimer>
#include <atomic>

#include "weather-api-controller.h"


namespace WeatherForecast
{
    WeatherApiController::WeatherApiController(Common::TaskManager* taskManager, Common::BackendConfig::ApiConfig apiConfig, QObject* parent)
        : WeatherForecast::IWeatherApiController(parent),
        _taskManager(taskManager),
        _apiConfig(apiConfig)
    {}

    void WeatherApiController::fetchData(RequestData requestData)
    {
        const QString errorTemplate = "GeoNamesApiController::fetchData => %1";
        const int MAX_ATTEMPTS = 4;

        static std::atomic<int> attempt = 0;
        int currentAttempt = ++attempt;

        if (currentAttempt > MAX_ATTEMPTS)
        {
            qDebug() << "HALLO!";

            emit errorOccurred(
                errorTemplate.arg("Server not responding")
            );

            attempt = 0;

            return;
        }

        const QUrl url = this->_buildRequestUrl(requestData);
        QNetworkRequest networkRequest(url);

        networkRequest.setAttribute(QNetworkRequest::Http2AllowedAttribute, true);
        networkRequest.setRawHeader("Connection", "Keep-Alive");
        networkRequest.setRawHeader("Keep-Alive", "timeout=30, max=10");

        QNetworkReply* reply = _networkManager.get(networkRequest);

        QTimer::singleShot(1000, reply, [this, reply, requestData, currentAttempt]() mutable {
            if (!reply->isFinished())
            {
                reply->abort();

                QTimer::singleShot(500, this, [this, requestData = std::move(requestData)]() mutable {
                    this->fetchData(std::move(requestData));
                });
            }
        });

        connect(reply, &QNetworkReply::finished, this,
            [
                this,
                reply,
                requestData = std::move(requestData),
                errorTemplate = std::move(errorTemplate)
            ]() mutable
            {
                if (reply->error() != QNetworkReply::NoError)
                {
                    QMetaObject::invokeMethod(this, [this, error = errorTemplate.arg(reply->errorString())]() {
                        emit errorOccurred(error);
                    }, Qt::QueuedConnection);

                    return;
                }

                _taskManager->runAsync(Common::Priority::High,
                [
                    controller = QPointer<WeatherApiController>(this),
                    payload = reply->readAll(),
                    requestData = std::move(requestData)
                ]() mutable
                {
                   if (!controller)
                    {
                       return;
                    }

                    FetchResult fetchResult;
                    fetchResult.requestData = std::move(requestData);

                    QString error = "";

                    controller->_handleRequestPayload(payload, fetchResult, error);

                    QMetaObject::invokeMethod(controller,
                        [controller, fetchResult = std::move(fetchResult), error = std::move(error)]()
                        {
                            if (!controller)
                            {
                                return;
                            }

                            error.isEmpty()
                                ? emit controller->dataFetched(fetchResult)
                                : emit controller->errorOccurred(error);
                        },
                        Qt::QueuedConnection
                    );
                });

                reply->deleteLater();
            }
        );
    }

    void WeatherApiController::_handleRequestPayload(const QByteArray &payload, FetchResult &fetchResult, QString &errorString) const
    {
        QJsonParseError jsonError;
        const QJsonDocument doc = QJsonDocument::fromJson(payload, &jsonError);

        if (jsonError.error != QJsonParseError::NoError)
        {
            errorString = jsonError.errorString();

            return;
        }

        QString parseError;

        fetchResult.data = _parseResponse(doc, parseError);

        if (!parseError.isEmpty())
        {
            errorString = std::move(parseError);
        }
    }


    QUrl WeatherApiController::_buildRequestUrl(const RequestData &requestData) const
    {
        const auto route = "/data/2.5/" + WeatherResource::toString(requestData.resource);
        const auto params = "?lat=%1&lon=%2&appid=%3";

        QString url = QString(_apiConfig.url + route + params)
            .arg(requestData.latitude, requestData.longitude, _apiConfig.key);

        return QUrl(url);
    }

    QList<WeatherData> WeatherApiController::_parseResponse(const QJsonDocument& doc, QString& errorString) const
    {
        if (!doc.isObject())
        {
            errorString = "Invalid JSON response";

            return {};
        }

        const QJsonObject root = doc.object();

        return root.contains("list")
         ? WeatherData::parseWeatherListByApiResponse(root["list"].toArray().toVariantList())
         : QList<WeatherData>{WeatherData::parseWeatherByApiResponseObj(root.toVariantMap())};
    }
}
