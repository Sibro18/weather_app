#include "geo-names-view-model.h"

namespace GeoNames
{
    GeoNamesViewModel::GeoNamesViewModel(GeoNamesManager* dataBridge, GeoNamesDataStorage* dataStorage, QObject *parent)
        : QObject{parent}, _dataBridge{dataBridge}, _dataStorage{dataStorage}
    {
        connect(
            _dataBridge,
            &GeoNamesManager::geoNamesFetched,
            this,
            &GeoNamesViewModel::dataFetchedHandler
        );
    }

    void GeoNamesViewModel::fetchDataByRequestAsync(const QVariantMap &requestData) const
    {
        const RequestData &request = RequestData::createFromQVariantMap(requestData);

        _dataBridge->fetchDataByRequestAsync(request);
    }

    void GeoNamesViewModel::fetchDataFromFileSystem(const QString &countryCode) const
    {
        _dataBridge->fetchDataFromFileSystem(countryCode);
    }

    QVariantList GeoNamesViewModel::getCountryList() const
    {
        QVariantList returnList;
        const auto countryList = _dataStorage->getCountryList();

        for (const auto &country : countryList)
        {
            returnList.append(_dataStorage->getCountryData(country));
        }

        return returnList;
    }

    void GeoNamesViewModel::dataFetchedHandler(GeoNames::FetchResult* fetchResult)
    {
        QVariantMap returnData;

        if (!fetchResult->errorString.isEmpty())
        {
            returnData["errorString"] = fetchResult->errorString;
        }
        else
        {
            _dataStorage->addData(fetchResult->data);
            // if (_dataStorage->addData(fetchResult->data))

            returnData = _dataStorage->getCountryData(fetchResult->data.first);
        }

        emit this->locationsFetched(returnData);
    }


}
