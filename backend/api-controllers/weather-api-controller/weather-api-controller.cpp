#include "weather-api-controller.h"

#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QUrl>
#include <QDebug>
#include <QPointer>
#include <QJsonArray>

namespace WeatherForecast
{
    WeatherApiController::WeatherApiController(const QString &apiKey, GeneralUtils::TaskManager* taskManager, QObject* parent)
        : QObject(parent),
        _apiKey(apiKey),
        _taskManager(taskManager)
    {}

    void WeatherApiController::fetchWeather(const RequestData &requestData)
    {
        const QUrl url(QString(_apiUrlTemplate)
            .arg(requestData.resource, requestData.latitude, requestData.longitude, _apiKey)
        );

        const QNetworkRequest request(url);

        QNetworkReply* reply = _networkManager.get(request);

        connect(reply, &QNetworkReply::finished, this, [this, reply, requestData]()
        {
            if (reply->error() != QNetworkReply::NoError)
            {
                FetchResult* fetchResult  = new FetchResult();

                fetchResult->requestData = std::move(requestData);
                fetchResult->errorString = std::move(reply->errorString());

                emit weatherFetched(fetchResult);
            }
            else
            {
                _taskManager->runAsync(GeneralUtils::Priority::High, [
                    controller  = QPointer<WeatherApiController>(this),
                    payload     = std::move(reply->readAll()),
                    requestData = std::move(requestData)] ()
                {
                    if (!controller)
                    {
                        return;
                    }

                    FetchResult* fetchResult = controller->_getFetchResult(payload, requestData);

                    QMetaObject::invokeMethod(controller, [controller, fetchResult = std::move(fetchResult)]() {
                        if (!controller)
                        {
                            return;
                        }

                        emit controller->weatherFetched(fetchResult);
                    }, Qt::QueuedConnection);
                });
            }

            reply->deleteLater();
        });
    }

    QString WeatherApiController::getFileNameByRequest(const RequestData &requestData) const
    {
        QString fileName = "";

        if (!requestData.resource.isEmpty())
        {
            fileName += requestData.resource + "_";
        }

        if (!requestData.latitude.isEmpty())
        {
            fileName += requestData.latitude + "_";
        }

        if (!requestData.longitude.isEmpty())
        {
            fileName += requestData.longitude;
        }

        fileName.replace(" ", "_");
        fileName.replace(",", ".");
        fileName.replace("/", "-");

        return fileName;
    }


    QList<WeatherData> WeatherApiController::_parseResponse(const QJsonDocument& doc, QString& errorString) const
    {
        if (!doc.isObject())
        {
            errorString = "Invalid JSON response";

            return {};
        }

        const QJsonObject root = doc.object();

        auto weatherDataList = root.contains("list")
         ? WeatherData::parseWeatherListByApiResponse(root["list"].toArray().toVariantList())
         : QList<WeatherData>{WeatherData::parseWeatherByApiResponseObj(root.toVariantMap())};

        return weatherDataList;
    }

    FetchResult* WeatherApiController::_getFetchResult(const QByteArray &payload, const RequestData &requestData) const
    {
        FetchResult* fetchResult = new FetchResult();

        fetchResult->requestData = requestData;

        QJsonParseError jsonError;
        const QJsonDocument doc = QJsonDocument::fromJson(payload, &jsonError);

        if (jsonError.error != QJsonParseError::NoError)
        {
            fetchResult->errorString = jsonError.errorString();

            return fetchResult;
        }

        QString parseError;

        fetchResult->data = std::move(_parseResponse(doc, parseError));

        if (!parseError.isEmpty())
        {
            fetchResult->errorString = std::move(parseError);
        }

        return fetchResult;
    }
}
