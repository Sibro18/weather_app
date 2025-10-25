#include "pressure-data.h"

namespace WeatherForecast
{
    QVariantMap PressureData::toVariantMap() const
    {
        return {
            {"seaLevel", seaLevel},
            {"groundLevel", groundLevel}
        };
    }

    PressureData PressureData::getByVariantMap(const QVariantMap &data)
    {
        return {
            .groundLevel = data["groundLevel"].toDouble(),
            .seaLevel = data["seaLevel"].toDouble()
        };
    }

    PressureData PressureData::getByVariantMapInApiResponse(const QVariantMap &data)
    {
        return {
            .groundLevel = data["grnd_level"].toDouble(),
            .seaLevel = data["sea_level"].toDouble()
        };
    }
}
