#ifndef I_WEATHER_FORECAST_CACHE_SERVICE_H
#define I_WEATHER_FORECAST_CACHE_SERVICE_H


#include "../../../core/interfaces/i-cache-service.h"
#include "../../../core/entities/weather-forecast/forecast-data/forecast-data.h"


namespace WeatherForecast
{
    using IWeatherForecastCacheService = ICacheService<WeatherForecast::ForecastData>;
}

#endif // I_WEATHER_FORECAST_CACHE_SERVICE_H
