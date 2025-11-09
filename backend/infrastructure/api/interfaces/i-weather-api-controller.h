#ifndef I_WEATHER_API_CONTROLLER_H
#define I_WEATHER_API_CONTROLLER_H


#include "../../../core/interfaces/i-api-controller.h"
#include "../../../core/entities/weather-forecast/request-data/request-data.h"
#include "../../../core/entities/weather-forecast/fetch-result/fetch-result.h"


namespace WeatherForecast
{
    /**
     * @brief Interface for weather API controllers handling forecast data requests.
     */
    class IWeatherApiController : public QObject, public IApiController<WeatherForecast::RequestData>
    {
        Q_OBJECT
    public:
        /**
         * @brief Create weatherApiController.
         * @param parent - parent object.
         */
        inline explicit IWeatherApiController(QObject *parent = nullptr)
            : QObject{parent}
        {}
    signals:
        /**
         * @brief Emitted when weather forecast data has been successfully fetched.
         * @param response - Fetch result containing forecast data.
         */
        void dataFetched(const WeatherForecast::FetchResult &response);

        /**
         * @brief Emitted when an error occurs during API request.
         * @param error - Human-readable error description.
         */
        void errorOccurred(const QString &error);
    };
}

#endif // I_WEATHER_API_CONTROLLER_H
