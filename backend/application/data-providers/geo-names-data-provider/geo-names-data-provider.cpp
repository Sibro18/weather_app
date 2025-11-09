#include <QSet>
#include <QFileInfo>
#include <QPointer>

#include "geo-names-data-provider.h"

namespace GeoNames
{
    GeoNamesDataProvider::GeoNamesDataProvider(
        GeoNames::ILocationsApiController* apiController,
        Common::TaskManager* taskManager,
        GeoNames::IGeoNamesCacheService* geoNamesCacheService,
        GeoNames::IGeoNamesService* geoNamesService,
        QObject* parent
    ) : QObject(parent),
        _apiController(apiController),
        _taskManager(taskManager),
        _geoNamesCacheService(geoNamesCacheService),
        _geoNamesService(geoNamesService)
    {
        connect(
            _apiController,
            &GeoNames::ILocationsApiController::dataFetched,
            this,
            &GeoNames::GeoNamesDataProvider::handleDataFromApiFetched
        );
    }
    void GeoNamesDataProvider::fetchDataByRequestAsync(GeoNames::RequestData requestData)
    {
        _taskManager->runAsync(
            Common::Priority::Normal,
            [
                controller = QPointer<GeoNamesDataProvider>(this),
                requestData = std::move(requestData)
            ] () {
                if (!controller)
                {
                    return;
                }

                if (requestData.name.isEmpty())
                {
                    auto data = controller->_geoNamesCacheService->get(requestData.countryCode);

                    if (!data)
                    {
                        data = controller->_geoNamesService->getLocationData(requestData.countryCode, std::nullopt);
                    }

                    if (data)
                    {
                        controller->_geoNamesCacheService->set(requestData.countryCode, *data);

                        FetchResult fetchResult;
                        fetchResult.data = std::make_pair(requestData.countryCode, std::move(*data));
                        fetchResult.requestData = std::move(requestData);

                        QMetaObject::invokeMethod(controller, [controller = controller, fetchResult = std::move(fetchResult)]() {
                            if (!controller)
                            {
                                return;
                            }

                            emit controller->geoNamesFetched(fetchResult);
                        }, Qt::QueuedConnection);

                        return;
                    }
                }

                QMetaObject::invokeMethod(controller, [controller = controller, requestData = std::move(requestData)]() {
                    if (!controller)
                    {
                        return;
                    }

                    controller->_apiController->fetchData(std::move(requestData));
                }, Qt::QueuedConnection);
            }
        );
    }

    QStringList GeoNamesDataProvider::getCountryList() const
    {
        auto data = _geoNamesCacheService->getCountryList();

        if (data.empty()) {
            data = _geoNamesService->getCountryCodeList(std::nullopt);
        }

        return data;
    }

    void GeoNamesDataProvider::handleDataFromApiFetched(const GeoNames::FetchResult& fetchResult)
    {
        const auto& countryCode = fetchResult.data.first;
        const auto& countryLocationsList = fetchResult.data.second;

        if (!countryCode.isEmpty() && !countryLocationsList.isEmpty())
        {
            _geoNamesService->addCountryLocations(countryCode, countryLocationsList);
            _geoNamesCacheService->set(countryCode, countryLocationsList);
        }

        emit geoNamesFetched(fetchResult);
    }
}
    
