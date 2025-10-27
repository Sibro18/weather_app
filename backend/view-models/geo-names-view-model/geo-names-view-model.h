#ifndef GEO_NAMES_VIEW_MODEL_H
#define GEO_NAMES_VIEW_MODEL_H

#include <QObject>
#include <QVariantList>
#include <QMap>

#include "../../managers/geo-names-manager/geo-names-manager.h"
#include "../../data-storage/geo-names-data-storage/geo-names-data-storage.h"


namespace GeoNames
{
    class GeoNamesViewModel : public QObject
    {
        Q_OBJECT
    public:
        explicit GeoNamesViewModel(GeoNamesManager* dataBridge, GeoNamesDataStorage* dataStorage, QObject *parent = nullptr);

        Q_INVOKABLE void fetchDataByRequestAsync(const QVariantMap &requestData);
        Q_INVOKABLE void fetchDataFromFileSystem(const QString &countryCode) const;
        Q_INVOKABLE QVariantList getCountryList() const;

        void dataFetchedHandler(GeoNames::FetchResult* fetchResult);
    signals:
        void locationsFetched(QVariantMap dataList);
    private:
        GeoNamesManager* _dataBridge;
        GeoNamesDataStorage* _dataStorage;
        QMap<RequestData, QVariantMap> _requestsHistory;
    };
} // namespace GeoNames

#endif // GEO_NAMES_VIEW_MODEL_H
