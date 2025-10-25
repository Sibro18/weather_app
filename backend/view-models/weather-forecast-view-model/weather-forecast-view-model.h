#ifndef WEATHER_FORECAST_VIEW_MODEL_H
#define WEATHER_FORECAST_VIEW_MODEL_H


#include <QString>
#include <QList>
#include <QPair>

#include "../../api-controllers/weather-api-controller/weather-api-controller.h"
#include "../../utils/task-manager/task-manager.h"
#include "../../data-storage/weather-forecast-data-storage/weather-forecast-data-storage.h"


namespace WeatherForecast
{
    /**
     * @brief Bridge between the Weather API controller and the rest of the application.
     */
    class WeatherForecastViewModel : public QObject
    {
        Q_OBJECT
    public:
        /**
         * @brief Constructs a WeatherDataBridge instance.
         * @param apiController - Pointer to the WeatherApiController responsible for API calls.
         * @param taskManager - Pointer to TaskManager for asynchronous task execution.
         * @param fileService - Pointer to FileService for file system operations.
         * @param parent - Optional parent QObject.
         */
        explicit WeatherForecastViewModel(
            WeatherForecast::WeatherApiController* apiController,
            GeneralUtils::TaskManager* taskManager,
            WeatherForecast::WeatherForecastDataStorage* dataStorage,
            QObject* parent = nullptr
        );

        /**
         * @brief Fetches weather data for the given request parameters.
         * @param requestData - Parameters describing the weather request.
         */
        Q_INVOKABLE void fetchDataAsync(QVariantMap requestData);

        Q_INVOKABLE int getAvgTemperatureOffTheDay(const QVariantList &variantList) const;

        Q_INVOKABLE QVariantMap getAvgTemperatureRange(const QVariantList &variantList) const;
    signals:
        /**
         * @brief Emitted when weather data has been fetched and parsed successfully.
         * @param fetchResult - The result of the fetch operation.
         */
        void weatherFetched(QVariantMap fetchResult);
    private:
        /**
         * @brief Pointer to the task manager for running asynchronous operations.
         */
        GeneralUtils::TaskManager* _taskManager;

        /**
         * @brief Pointer to the API controller for making network requests.
         */
        WeatherApiController* _apiController;

        WeatherForecastDataStorage _dataStorage;

        bool _verifyOnExpired(const ForecastData& forecastData, const QString& requestResource) const;

        QPair<int, int> _getMinMaxTemperatures(QList<WeatherData> data) const;
    private slots:
        /**
         * @brief Handles data received from the API controller.
         * @param fetchResult - The result received from the API.
         */
        void handleDataFromApiFetched(WeatherForecast::FetchResult* fetchResult);
    };
}

#endif // WEATHER_FORECAST_VIEW_MODEL_H
