#include "wind-data.h"

namespace WeatherForecast
{
    QVariantMap WindData::toVariantMap() const
    {
        return {
            {"speed", speed},
            {"degrees", degrees},
            {"gust", gust}
        };
    }

    WindData WindData::getByVariantMap(const QVariantMap &data)
    {
        return {
            .speed = data["speed"].toDouble(),
            .degrees = data["degrees"].toDouble(),
            .gust = data["gust"].toDouble()
        };
    }

    WindData WindData::getByVariantMapInApiResponse(const QVariantMap &data)
    {
        return {
            .speed = data["speed"].toDouble(),
            .degrees = data["deg"].toDouble(),
            .gust = data["gust"].toDouble()
        };
    }
}
