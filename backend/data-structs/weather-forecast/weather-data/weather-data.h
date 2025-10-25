#ifndef WEATHER_DATA_H
#define WEATHER_DATA_H


#include <QString>
#include <QJsonObject>
#include <QVariantMap>

#include "../../weather-forecast/weather-sub-structs/weather-short-description/weather-short-description.h"
#include "../../weather-forecast/weather-sub-structs/wind-data/wind-data.h"
#include "../../weather-forecast/weather-sub-structs/temperature-data/temperature-data.h"
#include "../../weather-forecast/weather-sub-structs/pressure-data/pressure-data.h"


namespace WeatherForecast
{
    struct WeatherData
    {
        WeatherData() = default;
        WeatherForecast::WeatherShortDescription weatherShortDescription;
        QString longDescription;

        TemperatureData temperatureData;
        PressureData pressureData;
        WindData windData;

        double clouds; // percents.
        double humidity; // percents.
        double visibility; // meters.
        std::optional<QPair<double, QString>> rain; // mm.
        std::optional<QPair<double, QString>> snow; // mm.
        qint64 unixTime;

        /**
         * @brief Converts this object into a QJsonObject.
         *
         * @return QJsonObject containing the serialized field data.
         */
        QJsonObject toJson() const;

        static WeatherData parseWeatherByApiResponseObj(const QVariantMap& data);
        static QList<WeatherData> parseWeatherListByApiResponse(const QVariantList& dataList);

        /**
         * @brief Convert to json view.
         * @return QVariantMap serialized GeoParsingData.
         */
        QVariantMap toVariantMap() const;

        /**
         * @brief Get GeoParsingData by parsing json View.
         * @param data - QVariantMap serialized GeoParsingData.
         * @return Reconstructed CountryLocationsData object.
         */
        static WeatherData getByVariantMap(const QVariantMap &data);

        static QList<WeatherData> getByVariantList(const QVariantList& dataList);
    };
}

#endif // WEATHER_DATA_H
