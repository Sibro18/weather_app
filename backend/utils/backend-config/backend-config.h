#ifndef BACKEND_CONFIG_H
#define BACKEND_CONFIG_H


namespace Common
{
    /**
     * @brief Centralized configuration manager for backend services.
     */
    class BackendConfig
    {
    public:
        /**
         * @brief web API configuration.
         */
        struct ApiConfig
        {
            QString url; ///< Base URL for API endpoint.
            QString key; ///< API key or authentication token.
        };

        /**
         * @brief Filesystem paths configuration.
         */
        struct FileSystemConfig
        {
            QString relativePath; ///< Path relative to application directory.
            QString absolutePath; ///< Fully qualified absolute path.
        };

        /**
         * @brief Complete service configuration bundle.
         */
        struct ServiceConfig
        {
            ApiConfig api;               ///< API connection settings.
            FileSystemConfig fileSystem; ///< Filesystem path settings.
        };

        /**
         * @brief Get GeoNames service configuration.
         * @return ServiceConfig for GeoNames API and file paths.
         */
        inline static ServiceConfig geoNames() { return BackendConfig::_geoNames; }

        /**
         * @brief Get weather forecast service configuration.
         * @return ServiceConfig for weather API and file paths.
         */
        inline static ServiceConfig weatherForecast() { return BackendConfig::_weather; }

        /**
         * @brief Get cache timeout setting.
         * @return Cache timeout value in seconds.
         */
        inline static int cacheTimeout() { return BackendConfig::_cacheTimeout; }

        /**
         * @brief Load configurations from JSON configuration file.
         * @param filename - Path to configuration file.
         * @return True if configuration was successfully loaded, false otherwise.
         */
        static bool load(const QString& filename);
    private:
        inline static ServiceConfig _geoNames;     ///< GeoNames service configuration.
        inline static ServiceConfig _weather;      ///< Weather service configuration.

        inline static int _cacheTimeout = 3600; ///< Default cache timeout: 1 hour.
    };
}

#endif // BACKEND_CONFIG_H
