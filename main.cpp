#include "backend/infrastructure/api/geo-names-api-controller/geo-names-api-controller.h"
#include "backend/infrastructure/cache/geo-names-cache-service/geo-names-cache-service.h"



#include "backend/application/data-providers/geo-names-data-provider/geo-names-data-provider.h"
#include "backend/application/services/geo-names/geo-names-file-service/geo-names-file-service.h"
#include "backend/view-models/geo-names-view-model/geo-names-view-model.h"

#include "backend/infrastructure/api/weather-api-controller/weather-api-controller.h"
#include "backend/infrastructure/cache/weather-forecast-cache-service/weather-forecast-cache-service.h"
#include "backend/application/data-providers/weather-forecast-data-provider/weather-forecast-data-provider.h"
#include "backend/view-models/weather-forecast-view-model/weather-forecast-view-model.h"

// #include "backend/view-models/weather-forecast-view-model/weather-forecast-view-model.h"

#include "backend/utils/backend-config/backend-config.h"
#include "backend/utils/file-service/file-service.h"

#include <QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication* app(new QGuiApplication {argc, argv});

    GeneralUtils::BackendConfig::load(":/configs/backend");

    GeneralUtils::TaskManager* taskManager = new GeneralUtils::TaskManager(app);
    GeneralUtils::FileService* fileService = new GeneralUtils::FileService(app);

    GeoNames::GeoNamesCacheService* geoCacheService = new GeoNames::GeoNamesCacheService(app);
    GeoNames::GeoNamesApiController* geoApiController = new GeoNames::GeoNamesApiController(
        taskManager,
        GeneralUtils::BackendConfig::geoNames().api,
        app
    );
    GeoNames::GeoNamesFileService* geoFileService = new GeoNames::GeoNamesFileService(
        fileService,
        { GeneralUtils::BackendConfig::geoNames().fileSystem.relativePath, false },
        app
    );
    GeoNames::GeoNamesDataProvider* geoNamesBridge = new GeoNames::GeoNamesDataProvider(
        geoApiController,
        taskManager,
        geoCacheService,
        geoFileService,
        app
    );
    GeoNames::GeoNamesViewModel* geoNamesController = new GeoNames::GeoNamesViewModel(
        geoNamesBridge,
        app
    );

    WeatherForecast::WeatherApiController* weatherApiController = new WeatherForecast::WeatherApiController(
        taskManager,
        GeneralUtils::BackendConfig::weatherForecast().api,
        app
    );
    WeatherForecast::WeatherForecastCacheService* weatherDataStorage = new WeatherForecast::WeatherForecastCacheService(app);
    WeatherForecast::WeatherForecastDataProvider* weatherForecastDataProvider = new WeatherForecast::WeatherForecastDataProvider(
        weatherApiController,
        weatherDataStorage,
        app
    );
    WeatherForecast::WeatherForecastViewModel* weatherForecastController = new WeatherForecast::WeatherForecastViewModel(
        weatherForecastDataProvider,
        app
    );

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("geoNamesController", geoNamesController);
    engine.rootContext()->setContextProperty("weatherForecastController", weatherForecastController);

    engine.load(QUrl(QStringLiteral("/home/argento/learning/weather_app/frontend/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app->exec();
}

