#include "weather-short-description.h"


namespace WeatherForecast
{
    WeatherShortDescription::WeatherShortDescription()
        : _code(Code::Clear)
    {}

    WeatherShortDescription::WeatherShortDescription(Code code)
        : _code(code)
    {}

    QString WeatherShortDescription::toString() const
    {
        return _codeToString(_code);
    }

    WeatherShortDescription WeatherShortDescription::fromString(const QString &s)
    {
        static const QMap<QString, Code> map =
        {
            {"Thunderstorm", Code::Thunderstorm},
            {"Drizzle",      Code::Drizzle},
            {"Rain",         Code::Rain},
            {"Snow",         Code::Snow},
            {"Mist",         Code::Mist},
            {"Smoke",        Code::Smoke},
            {"Haze",         Code::Haze},
            {"Dust",         Code::Dust},
            {"Fog",          Code::Fog},
            {"Sand",         Code::Sand},
            {"Ash",          Code::Ash},
            {"Squall",       Code::Squall},
            {"Tornado",      Code::Tornado},
            {"Clear",        Code::Clear},
            {"Clouds",       Code::Clouds}
        };

        auto it = map.find(s);

        if (it != map.end())
        {
            return WeatherShortDescription(it.value());
        }

        throw std::invalid_argument("Unknown WeatherMain string");
    }

    WeatherShortDescription::Code WeatherShortDescription::code() const
    {
        return _code;
    }

    QString WeatherShortDescription::_codeToString(Code code)
    {
        switch (code)
        {
            case Code::Thunderstorm: return "Thunderstorm";
            case Code::Drizzle:      return "Drizzle";
            case Code::Rain:         return "Rain";
            case Code::Snow:         return "Snow";
            case Code::Mist:         return "Mist";
            case Code::Smoke:        return "Smoke";
            case Code::Haze:         return "Haze";
            case Code::Dust:         return "Dust";
            case Code::Fog:          return "Fog";
            case Code::Sand:         return "Sand";
            case Code::Ash:          return "Ash";
            case Code::Squall:       return "Squall";
            case Code::Tornado:      return "Tornado";
            case Code::Clear:        return "Clear";
            case Code::Clouds:       return "Clouds";
            default:
                break;
        }

        return {};
    }
}
