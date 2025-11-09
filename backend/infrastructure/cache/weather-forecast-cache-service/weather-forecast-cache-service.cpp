#include "weather-forecast-cache-service.h"
#include "../../../core/entities/weather-forecast/request-data/request-data.h"


namespace WeatherForecast
{
    WeatherForecastCacheService::WeatherForecastCacheService(QObject *parent)
        : QObject{parent}
    {}

    std::optional<ForecastData> WeatherForecastCacheService::get(const QString& key)
    {
        auto idx = _weatherForecastMap.find(key);

        if (idx == _weatherForecastMap.end())
        {
            return std::nullopt;
        }

        auto &value = (*idx);

        if (!value.isValid(WeatherResource::CurrentWeather))
        {
            value.hourForecast = std::nullopt;
        }

        if (!value.isValid(WeatherResource::FiveDayForecast))
        {
            value.fiveDaysForecast = std::nullopt;
        }

        if (!value.hourForecast && !value.fiveDaysForecast)
        {
            return std::nullopt;
        }

        return { value };
    }

    void WeatherForecastCacheService::set(const QString& key, ForecastData data)
    {
        auto idx = _weatherForecastMap.find(key);

        if (idx == _weatherForecastMap.end())
        {
            _weatherForecastMap.insert(key, std::move(data));

            return;
        }

        if (data.hourForecast)
        {
            (*idx).hourForecast = data.hourForecast;
        }

        if (data.fiveDaysForecast)
        {
            (*idx).fiveDaysForecast = data.fiveDaysForecast;
        }
    }

    void WeatherForecastCacheService::remove(const QString& key)
    {
        _weatherForecastMap.remove(key);
    }

    void WeatherForecastCacheService::clear()
    {
        _weatherForecastMap.clear();
    }

    bool WeatherForecastCacheService::contains(const QString& key)
    {
        return _weatherForecastMap.contains(key);
    }
}
