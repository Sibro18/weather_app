#ifndef WEATHER_FORECAST_FETCH_RESULT_H
#define WEATHER_FORECAST_FETCH_RESULT_H


#include "../request-data/request-data.h"
#include "../weather-data/weather-data.h"


namespace WeatherForecast
{
    /**
     * @brief Result of fetching and parsing wheather data.
     */
    class FetchResult
    {
    public:
        /**
         * @brief Default constructor.
         */
        FetchResult() = default;

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
