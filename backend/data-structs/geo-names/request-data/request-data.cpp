#include "request-data.h"


namespace GeoNames
{
    RequestData RequestData::createFromQVariantMap(const QVariantMap &requestData)
    {
        RequestData request;

        request.countryCode = requestData.value("countryCode", "").toString();
        request.name = requestData.value("name", "").toString();
        request.startWith = requestData.value("startWith", false).toBool();
        request.lang = requestData.value("lang", "en").toString();
        request.featureClass = { FeatureClass::fromString(requestData.value("featureClass", "P").toString()) };

        return request;
    }

    QVariantMap RequestData::toVariantMap() const
    {
        QVariantMap map;
        map["name"] = name;
        map["countryCode"] = countryCode;
        map["startWith"] = startWith;
        map["lang"] = lang;

        if (featureClass)
        {
            map["featureClass"] = featureClass.value().toString();
        }

        return map;
    }
}
