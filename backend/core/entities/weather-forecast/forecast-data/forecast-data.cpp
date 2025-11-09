#include "forecast-data.h"


namespace WeatherForecast
{
    ForecastData::ForecastData(const QList<WeatherData>& data, const std::chrono::system_clock::time_point& timePoint)
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

    QVariantMap ForecastData::toVariantMap() const
    {
        QVariantMap returnData;

        if (fiveDaysForecast != std::nullopt)
        {
            QList<WeatherData> forecast = fiveDaysForecast.value().second;
            QMap<QString, QVariantList> daysMap;

            for (const auto& weatherData : forecast)
            {
                if (weatherData.dateIsoFormat)
                {
                    QString date = weatherData.dateIsoFormat->split(" ")[0];
                    daysMap[date].append(weatherData.toVariantMap());
                }
            }

            QVariantList fiveDaysForecastVariant;
            int dayCount = 0;

            for (auto it = daysMap.begin(); it != daysMap.end() && dayCount < 5; ++it, ++dayCount)
            {
                QVariantMap dayObject;
                dayObject["name"] = it.key();
                dayObject["items"] = it.value();
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

    bool ForecastData::isValid(const WeatherResource::Type& requestResource) const
    {
        using namespace std::chrono;

        auto now = system_clock::now();
        std::time_t now_t = system_clock::to_time_t(now);
        std::tm* local_tm = std::localtime(&now_t);

        int current_year = local_tm->tm_year + 1900;
        int current_month = local_tm->tm_mon + 1;
        int current_day = local_tm->tm_mday;
        int current_hour = local_tm->tm_hour;

        if (requestResource == WeatherResource::Type::CurrentWeather && this->hourForecast)
        {
            const auto& hourForecast = *this->hourForecast;

            std::time_t forecast_t = system_clock::to_time_t(hourForecast.first);
            std::tm* forecast_tm = std::localtime(&forecast_t);

            int forecast_year = forecast_tm->tm_year + 1900;
            int forecast_month = forecast_tm->tm_mon + 1;
            int forecast_day = forecast_tm->tm_mday;
            int forecast_hour = forecast_tm->tm_hour;

            return (current_year == forecast_year &&
                current_month == forecast_month &&
                current_day == forecast_day &&
                current_hour == forecast_hour
            );
        }
        else if (this->fiveDaysForecast && requestResource == WeatherResource::Type::FiveDayForecast)
        {
            const auto& fiveDaysForecast = *this->fiveDaysForecast;

            std::time_t forecast_t = system_clock::to_time_t(fiveDaysForecast.first);
            std::tm* forecast_tm = std::localtime(&forecast_t);

            int forecast_year = forecast_tm->tm_year + 1900;
            int forecast_month = forecast_tm->tm_mon + 1;
            int forecast_day = forecast_tm->tm_mday;

            return (current_year == forecast_year &&
                current_month == forecast_month &&
                current_day == forecast_day
            );
        }

        return false;
    }
}
