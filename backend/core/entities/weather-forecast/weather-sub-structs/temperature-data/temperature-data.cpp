#include "temperature-data.h"


namespace WeatherForecast
{
    QVariantMap TemperatureData::toVariantMap() const
    {
        return
        {
            {"temperature", temperature},
            {"feelsLike", feelsLike},
            {"min", min},
            {"max", max}
        };
    }

    TemperatureData TemperatureData::getByVariantMap(const QVariantMap &data)
    {
        return
        {
            .temperature = data["temperature"].toInt(),
            .feelsLike = data["feelsLike"].toInt(),
            .min = data["min"].toInt(),
            .max = data["max"].toInt()
        };
    }

    TemperatureData TemperatureData::getByVariantMapInApiResponse(const QVariantMap &data)
    {
        const double kelvinZero = 273.15;

        auto computeCelsiusDegree = [&kelvinZero] (const double& kelvinTemperature)
        {
            return static_cast<int>(kelvinTemperature - kelvinZero);
        };

        return
        {
            .temperature = computeCelsiusDegree(data["temp"].toDouble()),
            .feelsLike = computeCelsiusDegree(data["feels_like"].toDouble()),
            .min = computeCelsiusDegree(data["temp_min"].toDouble()),
            .max = computeCelsiusDegree(data["temp_max"].toDouble())
        };
    }
}
