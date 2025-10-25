#ifndef WEATHER_API_CONTROLLER_H
#define WEATHER_API_CONTROLLER_H


#include <QString>
#include <QtNetwork/QNetworkAccessManager>
#include <QJsonObject>
#include <QMap>

#include "../../data-structs/weather-forecast/fetch-result/fetch-result.h"
#include "../../data-structs/weather-forecast/request-data/request-data.h"
#include "../../utils/task-manager/task-manager.h"


namespace WeatherForecast
{
    /**
     * @brief Controller for interacting with the OpenWeather API.
     */
    class WeatherApiController : public QObject {
        Q_OBJECT
    public:
        /**
         * @brief Constructs the controller with the specified API key.
         * @param apiKey - OpenWeather API key.
         * @param parent - Optional parent QObject.
         */
        explicit WeatherApiController(const QString &apiKey, GeneralUtils::TaskManager* taskManager, QObject* parent = nullptr);

        /**
         * @brief Sends a request to the OpenWeather API using the given parameters.
         * @param requestData - Parameters for the weather request.
         */
        void fetchWeather(const WeatherForecast::RequestData &requestData);

        /**
         * @brief Generates a filename based on the request parameters.
         * @param requestData - Parameters used to build the filename.
         * @return Suggested filename for caching or saving the response.
         */
        QString getFileNameByRequest(const WeatherForecast::RequestData &requestData) const;
    signals:
        /**
         * @brief Emitted when weather data has been fetched.
         * @param data - Parsed result including original request and any error information.
         */
        void weatherFetched(WeatherForecast::FetchResult* data);
    private:
        /**
         * @brief OpenWeather API key used for authentication.
         */
        QString _apiKey;

        /**
         * @brief URL template for constructing API requests.
         */
        const QString _apiUrlTemplate = "https://api.openweathermap.org/data/2.5/%1?lat=%2&lon=%3&appid=%4";

        // forecast || weather

        /**
         * @brief Network manager for handling HTTP requests.
         */
        QNetworkAccessManager _networkManager;

        GeneralUtils::TaskManager* _taskManager;

        QList<WeatherData> _parseResponse(const QJsonDocument& doc, QString& errorString) const;

        /**
         * @brief Handles ther network reply.
         * @param reply - API response.
         * @param requestData - Original request data.
         * @return result of fetching containing parsed data and error information.
         */
        FetchResult* _getFetchResult(const QByteArray &payload, const RequestData &requestData) const;
    };
}

#endif // WEATHER_API_CONTROLLER_H
