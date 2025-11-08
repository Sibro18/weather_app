#ifndef WEATHER_SHORT_DESCRIPTION_H
#define WEATHER_SHORT_DESCRIPTION_H


namespace WeatherForecast
{
    /**
     * @brief Short weather condition codes based on OpenWeatherMap API.
     *
     * Represents standardized weather condition categories with mapping.
     * to OpenWeatherMap weather condition IDs.
     */
    class WeatherShortDescription
    {
    public:
        /**
         * @brief Weather condition codes with OpenWeatherMap ID ranges.
         */
        enum class Code
        {
            Thunderstorm, ///< Thunderstorm weather (ID range: 200-232)
            Drizzle,      ///< Drizzle precipitation (ID range: 300-321)
            Rain,         ///< Rain precipitation (ID range: 500-531)
            Snow,         ///< Snow precipitation (ID range: 600-622)
            Mist,         ///< Mist conditions (ID: 701)
            Smoke,        ///< Smoke conditions (ID: 711)
            Haze,         ///< Haze conditions (ID: 721)
            Dust,         ///< Dust conditions (ID: 731)
            Fog,          ///< Fog conditions (ID: 741)
            Sand,         ///< Sand conditions (ID: 751)
            Ash,          ///< Volcanic ash (ID: 761)
            Squall,       ///< Squall conditions (ID: 771)
            Tornado,      ///< Tornado conditions (ID: 781)
            Clear,        ///< Clear sky (ID: 800)
            Clouds        ///< Cloudy conditions (ID range: 801-804)
        };

        /**
         * @brief Default constructor.
         */
        explicit WeatherShortDescription();

        /**
         * @brief Construct with specific weather code.
         * @param code - Weather condition code.
         */
        explicit WeatherShortDescription(WeatherShortDescription::Code code);

        /**
         * @brief Get string representation of weather condition.
         * @return Human-readable weather condition string.
         */
        QString toString() const;

        /**
         * @brief Get the underlying weather code.
         * @return Current weather condition code.
         */
        Code code() const;

        /**
         * @brief Create WeatherShortDescription from string.
         * @param s - String representation of weather condition.
         * @return Corresponding WeatherShortDescription object.
         */
        static WeatherShortDescription fromString(const QString &s);
    private:
        Code _code; ///< Internal weather condition code.

        /**
         * @brief Convert weather code to string representation.
         * @param code - Weather condition code to convert.
         * @return String representation of the code.
         */
        static QString _codeToString(WeatherShortDescription::Code code);
    };
}

#endif // WEATHER_SHORT_DESCRIPTION_H
