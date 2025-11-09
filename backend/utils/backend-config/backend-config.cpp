#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>

#include "backend-config.h"


namespace Common
{
    bool BackendConfig::load(const QString& filename)
    {
        QFile file(filename);

        if (!file.open(QIODevice::ReadOnly))
        {
            return false;
        }

        QByteArray data = file.readAll();

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

        if (doc.isNull())
        {
            return false;
        }

        auto setData = [] (ServiceConfig &config, const QJsonObject &object)
        {
            if (object.contains("API"))
            {
                QJsonObject objectApi = object["API"].toObject();

                config.api.url = objectApi["URL"].toString();
                config.api.key = objectApi["KEY"].toString();
            }

            if (object.contains("FILE_SYSTEM"))
            {
                QJsonObject objectApi = object["FILE_SYSTEM"].toObject();

                config.fileSystem.relativePath = objectApi["RELATIVE"].toString();
                config.fileSystem.absolutePath = objectApi["ABSOLUTE"].toString();
            }
        };

        QJsonObject config = doc.object();

        setData(BackendConfig::_geoNames, config["GEO_NAMES"].toObject());
        setData(BackendConfig::_weather, config["WEATHER_FORECAST"].toObject());

        return true;
    }
}
