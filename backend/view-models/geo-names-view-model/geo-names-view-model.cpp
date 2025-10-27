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

    void GeoNamesViewModel::fetchDataByRequestAsync(const QVariantMap &requestData)
    {
        RequestData request = RequestData::createFromQVariantMap(requestData);

        auto it = _requestsHistory.find(request);
        if (it != _requestsHistory.end())
        {
            emit this->locationsFetched(it.value());

            return;
        }

        _dataBridge->fetchDataByRequestAsync(std::move(request));
    }

    void GeoNamesViewModel::fetchDataFromFileSystem(const QString &countryCode) const
    {
        _dataBridge->fetchDataFromFileSystem(countryCode);
    }

    QVariantList GeoNamesViewModel::getCountryList() const
    {
        QVariantList returnList;
        const QList<QString> countryList = _dataStorage->getCountryList();

        for (const QString &country : countryList)
        {
            returnList.append(_dataStorage->getCountryData(country));
        }

        return returnList;
    }

    void GeoNamesViewModel::dataFetchedHandler(GeoNames::FetchResult* fetchResult)
    {
        QScopedPointer<GeoNames::FetchResult> scopedResult(fetchResult);

        QVariantMap returnData;

        if (!scopedResult->errorString.isEmpty())
        {
            returnData["errorString"] = scopedResult->errorString;
        }
        else
        {
            _dataStorage->addData(scopedResult->data);

            returnData = _dataStorage->getCountryData(scopedResult->data.first);

            _requestsHistory[scopedResult->requestData] = returnData;
        }


        emit this->locationsFetched(returnData);
    }


}
