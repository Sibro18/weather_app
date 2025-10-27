#ifndef BACKEND_CONFIG_H
#define BACKEND_CONFIG_H

namespace GeneralUtils
{
    class BackendConfig
    {
    public:
        struct ApiConfig {
            QString url;
            QString key;
        };

        struct FileSystemConfig {
            QString relativePath;
            QString absolutePath;
        };

        struct ServiceConfig {
            ApiConfig api;
            FileSystemConfig fileSystem;
        };

        // Статические геттеры
        static ServiceConfig geoNames() { return BackendConfig::_geoNames; }
        static ServiceConfig weatherForecast() { return BackendConfig::_weather; }
        static int cacheTimeout() { return BackendConfig::_cacheTimeout; }

        static bool load(const QString& filename);

    private:
        inline static ServiceConfig _geoNames;
        inline static ServiceConfig _weather;
        inline static int _cacheTimeout = 3600;
    };
}

#endif // BACKEND_CONFIG_H
