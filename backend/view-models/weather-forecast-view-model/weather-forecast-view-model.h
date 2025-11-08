#ifndef WEATHER_FORECAST_VIEW_MODEL_H
#define WEATHER_FORECAST_VIEW_MODEL_H


#include "../../application/data-providers/weather-forecast-data-provider/weather-forecast-data-provider.h"


namespace WeatherForecast
{
    /**
     * @brief QML view model for weather forecast data presentation.
     */
    class WeatherForecastViewModel : public QObject
    {
        Q_OBJECT
    public:
        /**
         * @brief Constructs weather forecast view model.
         * @param dataProvider - Weather forecast data provider.
         * @param parent - Optional parent QObject.
         */
        explicit WeatherForecastViewModel(
            WeatherForecast::WeatherForecastDataProvider* dataProvider,
            QObject* parent = nullptr
        );

        /**
         * @brief Fetch weather data asynchronously from QML.
         * @param requestData - Request parameters as QVariantMap from QML.
         */
        Q_INVOKABLE void fetchDataAsync(const QVariantMap &requestData);

        /**
         * @brief Calculate average daily temperature.
         * @param variantList - List of weather data points.
         * @return Average temperature for the day.
         */
        Q_INVOKABLE int getAvgTemperatureOffTheDay(const QVariantList &variantList) const;

        /**
         * @brief Get daily temperature range.
         * @param variantList - List of weather data points.
         * @return Temperature range as QVariantMap with min/max.
         */
        Q_INVOKABLE QVariantMap getAvgTemperatureRange(const QVariantList &variantList) const;
    signals:
        /**
         * @brief Notify QML that weather data is ready.
         * @param fetchResult - Weather data as QVariantMap for QML.
         */
        void weatherFetched(QVariantMap fetchResult);
    private:
        WeatherForecastDataProvider* _dataProvider; ///< Weather forecast data provider.

        /**
         * @brief Calculate min and max temperatures from weather data.
         * @param data - List of weather data points.
         * @return Pair of min and max temperatures.
         */
        QPair<int, int> _getMinMaxTemperatures(QList<WeatherData> data) const;
    private slots:
        /**
         * @brief Handle fetched weather data from provider.
         * @param forecastData - Fetched weather forecast data.
         */
        void handleDataFetched(const WeatherForecast::ForecastData &forecastData);
    };
}

#endif // WEATHER_FORECAST_VIEW_MODEL_H
