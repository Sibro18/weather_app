#ifndef WEATHER_FORECAST_DATA_STORAGE_H
#define WEATHER_FORECAST_DATA_STORAGE_H

#include <QObject>
#include <QList>
#include <optional>
#include <chrono>

#include "../../data-structs/weather-forecast/weather-data/weather-data.h"

namespace WeatherForecast
{
    struct ForecastData
    {
        ForecastData() = default;

        ForecastData(const QList<WeatherData>& data, const std::chrono::system_clock::time_point& timePoint)
        {
            if (data.length() == 1)
            {
                hourForecast = std::make_pair(timePoint, data.first());
            }
            else if (data.length() > 0)
            {
                fiveDaysForecast = std::make_pair(timePoint, data);
            }
        }

        std::optional<QPair<std::chrono::system_clock::time_point, QList<WeatherData>>> fiveDaysForecast; // time of request, data;
        std::optional<QPair<std::chrono::system_clock::time_point, WeatherData>> hourForecast; // time of request, data;

        QVariantMap toVariantMap() const
        {
            QVariantMap returnData;

            if (fiveDaysForecast != std::nullopt)
            {
                QList<WeatherData> forecast = fiveDaysForecast.value().second;

                int dayCount = 5;
                int itemsPerDay = forecast.length() / dayCount;

                QVariantList fiveDaysForecastVariant;

                for (int day = 0; day < dayCount; day++)
                {
                    QVariantMap dayObject;
                    QVariantList dayForecast;

                    dayObject["name"] = "day " + QString::number(day + 1);

                    int startIndex = day * itemsPerDay;
                    int endIndex = day != dayCount - 1
                     ? startIndex + itemsPerDay
                     : forecast.length();

                    qDebug() << startIndex << " _ " << endIndex;
                    for (int i = startIndex; i < endIndex; i++)
                    {
                        dayForecast.append(forecast[i].toVariantMap());
                    }

                    dayObject["items"] = dayForecast;

                    fiveDaysForecastVariant.append(dayObject);
                }

                returnData["fiveDaysForecast"] = fiveDaysForecastVariant;
            }

            if (hourForecast != std::nullopt)
            {
                returnData["hourForecast"] = hourForecast.value().second.toVariantMap();
            }

            return returnData;
        }
    };

    class WeatherForecastDataStorage : public QObject
    {
        Q_OBJECT
    public:
        explicit WeatherForecastDataStorage(QObject *parent = nullptr);

        std::optional<ForecastData> getForecasetData(const QString& key)
        {
            if (!_weatherForecastMap.contains(key))
            {
                return std::nullopt;
            }

            return { _weatherForecastMap[key] };
        }

        void addForecastData(const QPair<QString, ForecastData>& data)
        {
            const QString& key = data.first;
            const ForecastData& forecastData = data.second;

            if (_weatherForecastMap.contains(key))
            {
                if (forecastData.hourForecast != std::nullopt)
                {
                    _weatherForecastMap[key].hourForecast = forecastData.hourForecast;
                }

                if (forecastData.fiveDaysForecast != std::nullopt)
                {
                    _weatherForecastMap[key].fiveDaysForecast = forecastData.fiveDaysForecast;
                }

                return;
            }

            _weatherForecastMap.insert(key, forecastData);
        }

    signals:

    private:
        QMap<QString, ForecastData> _weatherForecastMap;
    };
}

#endif // WEATHER_FORECAST_DATA_STORAGE_H
