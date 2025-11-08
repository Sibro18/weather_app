#include "request-data.h"


namespace WeatherForecast
{
    RequestData RequestData::createFromQVariantMap(const QVariantMap &requestData)
    {
        RequestData request;

        request.resource = WeatherResource::fromString(requestData.value("resource", "").toString());
        request.latitude = requestData.value("latitude", "").toString();
        request.longitude = requestData.value("longitude", "").toString();

        return request;
    }

    QVariantMap RequestData::toVariantMap() const
    {
        QVariantMap map;
        map["resource"] = WeatherResource::toString(resource);
        map["latitude"] = latitude;
        map["longitude"] = longitude;

        return map;
    }
}
