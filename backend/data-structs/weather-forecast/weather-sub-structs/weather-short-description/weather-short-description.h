#ifndef WEATHER_SHORT_DESCRIPTION_H
#define WEATHER_SHORT_DESCRIPTION_H

#include <QString>


namespace WeatherForecast
{
class WeatherShortDescription
{
public:
    enum class Code
    {
        Thunderstorm, // 200–232
        Drizzle,      // 300–321
        Rain,         // 500–531
        Snow,         // 600–622
        Mist,         // 701
        Smoke,        // 711
        Haze,         // 721
        Dust,         // 731
        Fog,          // 741
        Sand,         // 751
        Ash,          // 761
        Squall,       // 771
        Tornado,      // 781
        Clear,        // 800
        Clouds        // 801–804
    };

    explicit WeatherShortDescription();
    explicit WeatherShortDescription(WeatherShortDescription::Code code);
    QString toString() const;

    Code code() const;

    static WeatherShortDescription fromString(const QString &s);

private:
    Code _code;

    static QString _codeToString(WeatherShortDescription::Code code);
};
}

#endif // WEATHER_SHORT_DESCRIPTION_H
