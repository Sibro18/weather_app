#ifndef WEATHER_FORECAST_FETCH_RESULT_H
#define WEATHER_FORECAST_FETCH_RESULT_H


#include <QString>
#include <QList>
#include <QPair>
#include <QObject>
#include <QVariantMap>

#include "../request-data/request-data.h"
#include "../weather-data/weather-data.h"


namespace WeatherForecast
{
    /**
     * @brief Result of fetching and parsing wheather data.
     */
    class FetchResult : public QObject
    {
        Q_OBJECT
    public:
        /**
         * @brief Constructs the FetchResult.
         * @param parent - QObject.
         */
        explicit FetchResult(QObject* parent = nullptr);

        /**
         * @brief Error message if the request or parsing failed.
         */
        QString errorString;

        /**
         * @brief List of parsed weather data entries.
         */
        QList<WeatherData> data;

        /**
         * @brief Original request parameters used for the API call.
         */
        RequestData requestData;
    };
}
#endif // WEATHER_FORECAST_FETCH_RESULT_H
