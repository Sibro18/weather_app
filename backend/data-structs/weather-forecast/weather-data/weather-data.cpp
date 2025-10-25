#include "weather-data.h"

namespace WeatherForecast
{
    WeatherData WeatherData::parseWeatherByApiResponseObj(const QVariantMap& data)
    {
        WeatherData returnData;

        returnData.unixTime = data["dt"].toULongLong();
        returnData.visibility = data["visibility"].toDouble();

        if (data.contains("main") && data["main"].isValid())
        {
            QVariantMap main = data["main"].toMap();

            returnData.temperatureData = TemperatureData::getByVariantMapInApiResponse(main);
            returnData.pressureData    = PressureData::getByVariantMapInApiResponse(main);

            returnData.humidity = main["humidity"].toDouble();
        }

        if (data.contains("weather") && data["weather"].isValid())
        {
            QVariantList list = data["weather"].toList();

            if (!list.isEmpty())
            {
                QVariantMap weather = list.first().toMap();

                returnData.weatherShortDescription = WeatherShortDescription::fromString(weather["main"].toString());
                returnData.longDescription         = weather["description"].toString();
            }
        }

        if (data.contains("clouds") && data["clouds"].isValid())
        {
            returnData.clouds = data["clouds"].toMap()["all"].toDouble();
        }

        if (data.contains("wind") && data["wind"].isValid())
        {
            returnData.windData = WindData::getByVariantMapInApiResponse(data["wind"].toMap());
        }

        const QString hour3 = "3h";
        const QString hour1 = "1h";

        if (data.contains("rain") && data["rain"].isValid())
        {
            QVariantMap rainData = data["rain"].toMap();

            if (rainData.contains(hour3))
            {
                returnData.rain = { QPair<double, QString>(rainData[hour3].toDouble(), hour3) };
            }
            else if (rainData.contains(hour1))
            {
                returnData.rain = { QPair<double, QString>(rainData[hour1].toDouble(), hour1) };
            }
        }

        if (data.contains("snow") && data["snow"].isValid())
        {
            QVariantMap snowData = data["snow"].toMap();

            if (snowData.contains(hour3))
            {
                returnData.snow = { std::make_pair(snowData[hour3].toDouble(), hour3) };
            }
            else if (snowData.contains(hour1))
            {
                returnData.snow = { std::make_pair(snowData[hour1].toDouble(), hour1) };
            }
        }

        return returnData;
    }

    QList<WeatherData> WeatherData::parseWeatherListByApiResponse(const QVariantList& dataList)
    {
        QList<WeatherData> returnData;

        for (const auto& item : dataList)
        {
            returnData.append(
                parseWeatherByApiResponseObj(item.toMap())
            );
        }

        return returnData;
    }

    QVariantMap WeatherData::toVariantMap() const
    {
        QVariantMap returnObject;

        returnObject["shortDescription"] = weatherShortDescription.toString();
        returnObject["windData"] = windData.toVariantMap();
        returnObject["temperatureData"] = temperatureData.toVariantMap();
        returnObject["pressureData"] = pressureData.toVariantMap();
        returnObject["longDescription"] = longDescription;
        returnObject["clouds"] = clouds;
        returnObject["humidity"] = humidity;
        returnObject["visibility"] = visibility;
        returnObject["date"] = unixTime;

        if (rain.has_value())
        {
            QVariantMap rainData;
            const auto &rainOptValue = rain.value();

            rainData["metric"] = rainOptValue.second;
            rainData["quantity"] = rainOptValue.first;

            returnObject["rain"] = rainData;
        }

        if (snow.has_value())
        {
            QVariantMap snowData;
            const auto &snowOptValue = snow.value();

            snowData["metric"] = snowOptValue.second;
            snowData["quantity"] = snowOptValue.first;

            returnObject["snow"] = snowData;
        }

        return returnObject;
    }

    WeatherData WeatherData::getByVariantMap(const QVariantMap &data)
    {
        WeatherData returnData;

        try {
            returnData.weatherShortDescription = WeatherShortDescription::fromString(data["shortDescription"].toString());
        } catch(std::exception exception) {
            qDebug() << exception.what();
        }

        returnData.windData = WindData::getByVariantMap(data["windData"].toMap());
        returnData.temperatureData = TemperatureData::getByVariantMap(data["temperatureData"].toMap());
        returnData.pressureData = PressureData::getByVariantMap(data["pressureData"].toMap());
        returnData.longDescription = data["longDescription"].toString();
        returnData.clouds = data["clouds"].toInt();
        returnData.humidity = data["humidity"].toInt();

        returnData.visibility = data["visibility"].toInt();

        returnData.unixTime = data["date"].toLongLong();

        if (data.contains("rain"))
        {
            QVariantMap rainData = data["rain"].toMap();

            double quantity = rainData["quantity"].toDouble();
            QString metric = rainData["metric"].toString();

            returnData.rain = std::make_pair(quantity, metric);
        }

        if (data.contains("snow"))
        {
            QVariantMap snowData = data["snow"].toMap();

            double quantity = snowData["quantity"].toDouble();
            QString metric = snowData["metric"].toString();

            returnData.snow = std::make_pair(quantity, metric);
        }

        return returnData;
    }

    QList<WeatherData> WeatherData::getByVariantList(const QVariantList& dataList)
    {
        QList<WeatherData> returnData;

        for (const auto& item : dataList)
        {
            returnData.append(
                getByVariantMap(item.toMap())
            );
        }

        return returnData;
    }
}

/*
    void WeatherData::fillFromJsonObject(const QJsonObject& obj)
    {
        unixTime = obj.value("dt").toInteger();
        visibility = obj.value("visibility").toDouble();

        const QJsonObject &main = obj.value("main").toObject();

        if (obj.contains("main") && obj["main"].isObject())
        {
            QJsonObject mainObj = obj["main"].toObject();

            temperatureData.temperature          = mainObj.value("temp").toDouble();
            temperatureData.feelsLike = mainObj.value("feels_like").toDouble();
            temperatureData.min       = mainObj.value("temp_min").toDouble();
            temperatureData.max       = mainObj.value("temp_max").toDouble();

            humidity                 = mainObj.value("humidity").toDouble();
            pressureData.seaLevel    = mainObj.value("sea_level").toDouble();
            pressureData.groundLevel = mainObj.value("grnd_level").toDouble();
        }

        if (obj.contains("weather") && obj["weather"].isArray())
        {
            QJsonArray arr = obj["weather"].toArray();

            if (!arr.isEmpty() && arr.first().isObject())
            {
                QJsonObject wObj = arr.first().toObject();

                weatherShortDescription       = WeatherShortDescription::fromString(wObj.value("main").toString());
                longDescription   = wObj.value("description").toString();
            }
        }

        if (obj.contains("clouds") && obj["clouds"].isObject())
        {
            clouds = obj["clouds"].toObject().value("all").toDouble();
        }

        if (obj.contains("wind") && obj["wind"].isObject())
        {
            QJsonObject windObj = obj["wind"].toObject();
            windData.speed     = windObj.value("speed").toDouble();
            windData.degrees = windObj.value("deg").toDouble();
            windData.gust      = windObj.value("gust").toDouble();
        }

        if (obj.contains("rain") && obj["rain"].isObject())
        {
            auto rainObj =obj["rain"].toObject();
            QString hour3 = "3h";
            QString hour1 = "1h";

            if (rainObj.contains(hour3))
            {
                rain = QPair<double, QString>(rainObj.value(hour3).toDouble(), hour3);
            }
            else if (rainObj.contains(hour1))
            {
                rain = QPair<double, QString>(rainObj.value(hour1).toDouble(), hour1);
            }
        }

        if (obj.contains("snow") && obj["snow"].isObject())
        {
            auto snowObj =obj["snow"].toObject();
            QString hour3 = "3h";
            QString hour1 = "1h";

            if (snowObj.contains("3h"))
            {
                snow = QPair<double, QString>(snowObj.value(hour3).toDouble(), hour3);
            }
            else if (snowObj.contains("1h"))
            {
                snow = QPair<double, QString>(snowObj.value(hour1).toDouble(), hour1);
            }
        }
    }
*/
