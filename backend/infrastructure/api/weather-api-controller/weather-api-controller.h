#ifndef WEATHER_API_CONTROLLER_H
#define WEATHER_API_CONTROLLER_H


#include <QtNetwork/QNetworkAccessManager>

#include "../../../utils/task-manager/task-manager.h"
#include "../../../core/entities/weather-forecast/request-data/request-data.h"
#include "../../../core/entities/weather-forecast/fetch-result/fetch-result.h"
#include "../interfaces/i-weather-api-controller.h"
#include "../../../utils/backend-config/backend-config.h"


namespace WeatherForecast
{
    /**
     * @brief Controller for interacting with the OpenWeather API.
     */
    class WeatherApiController : public WeatherForecast::IWeatherApiController {
        Q_OBJECT
    public:
        /**
         * @brief Constructs the controller with the specified API key.
         * @param apiKey - OpenWeather API key.
         * @param parent - Optional parent QObject.
         */
        explicit WeatherApiController(Common::TaskManager* taskManager, Common::BackendConfig::ApiConfig apiConfig, QObject* parent = nullptr);

        /**
         * @brief Sends a request to the OpenWeather API using the given parameters.
         * @param requestData - Parameters for the weather request.
         */
        void fetchData(WeatherForecast::RequestData requestData) override;
    private:
        QNetworkAccessManager _networkManager; ///< Network manager for handling HTTP requests
        Common::TaskManager* _taskManager; ///< Manager for async operations.
        Common::BackendConfig::ApiConfig _apiConfig; ///< Config for API.

        /**
         * @brief get builded request URL.
         * @param requestData - parameters for API request.
         * @return Fully constructed Url for OpenWeatherMap API call.
         */
        QUrl _buildRequestUrl(const RequestData &requestData) const;

        /**
         * @brief Parses JSON response from OpenWeatherMap API.
         * @param doc - JSON document containing API response.
         * @param errorString - Reference to string where error message will be stored if parsing fails.
         * @return List of parsed OpenWeatherMap data objects.
         */
        QList<WeatherData> _parseResponse(const QJsonDocument& doc, QString& errorString) const;

        /**
         * @brief Handles ther network reply.
         * @param reply - API response.
         * @param requestData - Original request data.
         * @return result of fetching containing parsed data and error information.
         */
        void _handleRequestPayload(const QByteArray &payload, FetchResult &fetchResult, QString &errorString) const;
    };
}

#endif // WEATHER_API_CONTROLLER_H
