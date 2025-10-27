#include "backend/api-controllers/geo-names-api-controller/geo-names-api-controller.h"
#include "backend/managers/geo-names-manager/geo-names-manager.h"
#include "backend/data-storage/geo-names-data-storage/geo-names-data-storage.h"

#include "backend/view-models/geo-names-view-model/geo-names-view-model.h"
#include "backend/view-models/weather-forecast-view-model/weather-forecast-view-model.h"

#include "backend/data-storage/weather-forecast-data-storage/weather-forecast-data-storage.h"
#include "backend/api-controllers/weather-api-controller/weather-api-controller.h"
#include "backend/utils/backend-config/backend-config.h"

#include <QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>

int main(int argc, char *argv[])
{
    // ":/configs/app_config.json".
    QGuiApplication* app(new QGuiApplication {argc, argv});

    // Попробуй разные пути:
    qDebug() << GeneralUtils::BackendConfig::load(":/configs/backend");

    GeneralUtils::TaskManager* taskManager = new GeneralUtils::TaskManager(app);
    GeoNames::GeoNamesApiController* geoApiController = new GeoNames::GeoNamesApiController(taskManager, app);
    GeneralUtils::FileService* fileService = new GeneralUtils::FileService(app);

    GeoNames::GeoNamesManager* geoNamesBridge = new GeoNames::GeoNamesManager(geoApiController, taskManager, fileService, app);
    GeoNames::GeoNamesDataStorage* geoNamesDataStorage = new GeoNames::GeoNamesDataStorage(geoNamesBridge->getCountryList(), app);
    GeoNames::GeoNamesViewModel* geoNamesController = new GeoNames::GeoNamesViewModel(geoNamesBridge, geoNamesDataStorage, app);

    WeatherForecast::WeatherApiController* weatherApiController = new WeatherForecast::WeatherApiController(
        "66286377b1d5265be2cd40b16761b858",
        taskManager,
        app
    );
    WeatherForecast::WeatherForecastDataStorage* weatherDataStorage = new WeatherForecast::WeatherForecastDataStorage(app);
    WeatherForecast::WeatherForecastViewModel* weatherForecastController = new WeatherForecast::WeatherForecastViewModel(
        weatherApiController,
        taskManager,
        weatherDataStorage,
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
