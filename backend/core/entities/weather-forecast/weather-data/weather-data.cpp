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
                returnData.rain = { std::make_pair(rainData[hour3].toDouble(), hour3) };
            }
            else if (rainData.contains(hour1))
            {
                returnData.rain = { std::make_pair(rainData[hour1].toDouble(), hour1) };
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

        if (data.contains("dt_txt"))
        {
            returnData.dateIsoFormat = data["dt_txt"].toString();
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
        returnObject["date_unix"] = unixTime;

        if (dateIsoFormat)
        {
            returnObject["date_iso"] = *dateIsoFormat;
        }

        if (rain)
        {
            QVariantMap rainData;
            const auto &rainOptValue = rain.value();

            rainData["metric"] = rainOptValue.second;
            rainData["quantity"] = rainOptValue.first;

            returnObject["rain"] = rainData;
        }

        if (snow)
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

        try
        {
            returnData.weatherShortDescription = WeatherShortDescription::fromString(data["shortDescription"].toString());
        }
        catch(std::exception exception)
        {
            qDebug() << exception.what();
        }

        returnData.windData = WindData::getByVariantMap(data["windData"].toMap());
        returnData.temperatureData = TemperatureData::getByVariantMap(data["temperatureData"].toMap());
        returnData.pressureData = PressureData::getByVariantMap(data["pressureData"].toMap());
        returnData.longDescription = data["longDescription"].toString();
        returnData.clouds = data["clouds"].toInt();
        returnData.humidity = data["humidity"].toInt();

        returnData.visibility = data["visibility"].toInt();

        returnData.unixTime = data["date_unix"].toLongLong();

        if (data.contains("rain"))
        {
            returnData.dateIsoFormat = data["date_iso"].toString();
        }

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
