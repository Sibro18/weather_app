#include "geo-names-view-model.h"


namespace GeoNames
{
    GeoNamesViewModel::GeoNamesViewModel(GeoNamesDataProvider* dataProvider, QObject *parent)
        :
        QObject{parent},
        _dataProvider{dataProvider}
    {
        connect(
            _dataProvider,
            &GeoNamesDataProvider::geoNamesFetched,
            this,
            &GeoNamesViewModel::dataFetchedHandler
        );

        connect(
            _dataProvider,
            &GeoNamesDataProvider::apiErrorOcured,
            this,
            &GeoNamesViewModel::handleApiError
        );
    }

    void GeoNamesViewModel::fetchDataByRequestAsync(const QVariantMap &requestData)
    {
        RequestData request = RequestData::createFromQVariantMap(requestData);

        auto it = _requestsHistory.find(request);

        if (it != _requestsHistory.end())
        {
            emit this->locationsFetched(*it);

            return;
        }

        _dataProvider->fetchDataByRequestAsync(std::move(request));
    }

    QVariantList GeoNamesViewModel::getCountryList() const
    {
        QVariantList returnList;

        auto&& forIterrationList = _dataProvider->getCountryList();

        for (const auto& countryCode : forIterrationList)
        {
            QVariantMap returnItem;
            returnItem["key"] = countryCode;
            returnItem["mainTitle"] = countryCode;
            returnItem["items"] = QVariantList();

            returnList.append(returnItem);
        }

        return returnList;
    }

    void GeoNamesViewModel::dataFetchedHandler(const GeoNames::FetchResult &fetchResult)
    {
        auto returnData = _parseFetchedData(fetchResult.data);

        _requestsHistory[fetchResult.requestData] = returnData;

        emit this->locationsFetched(std::move(returnData));
    }

    void GeoNamesViewModel::handleApiError(const QString &error)
    {
        emit errorOccurred(error);
    }

    QVariantMap GeoNamesViewModel::_parseFetchedData(const QPair<QString, QList<LocationData>> & data) const
    {
        QVariantMap returnData;
        returnData["key"] = data.first;
        returnData["mainTitle"] = data.first;

        QVariantList cityList;

        for (const auto &item : data.second)
        {
            QVariantMap listItem;
            listItem["key"] = QString("%1_%2").arg(item.latitude, item.longitude);
            listItem["mainTitle"] = item.name;
            listItem["secondTitle"] = item.adminName;
            listItem["data"] = item.toVariantMap();

            cityList.append(listItem);
        }

        returnData["items"] = cityList;

        return returnData;
    }
}
