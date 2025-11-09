#ifndef GEO_NAMES_VIEW_MODEL_H
#define GEO_NAMES_VIEW_MODEL_H


#include "../../application/data-providers/geo-names-data-provider/geo-names-data-provider.h"


namespace GeoNames
{
    /**
     * @brief QML-friendly view model for GeoNames data presentation.
     */
    class GeoNamesViewModel : public QObject
    {
        Q_OBJECT
    public:
        /**
         * @brief Constructs GeoNames view model.
         * @param dataProvider - Data provider for GeoNames operations.
         * @param parent - Optional parent QObject.
         */
        explicit GeoNamesViewModel(GeoNamesDataProvider* dataProvider, QObject *parent = nullptr);

        /**
         * @brief Fetch GeoNames data asynchronously from QML.
         * @param requestData - Request parameters as QVariantMap from QML.
         */
        Q_INVOKABLE void fetchDataByRequestAsync(const QVariantMap &requestData);

        /**
         * @brief Get country list for QML presentation.
         * @return Country list as QVariantList for QML.
         */
        Q_INVOKABLE QVariantList getCountryList() const;
    signals:
        /**
         * @brief Notify QML that locations data is ready.
         * @param dataList - Locations data as QVariantMap for QML.
         */
        void locationsFetched(QVariantMap dataList);

        /**
         * @brief Notify QML that ocure error.
         * @param error - error in text view.
         */
        void errorOccurred(const QString &error);
    private:
        GeoNamesDataProvider* _dataProvider; ///< Data provider for GeoNames operations.
        QMap<RequestData, QVariantMap> _requestsHistory; ///< History of previous requests.

        /**
         * @brief Parse fetched data for QML presentation.
         * @param data - Raw GeoNames data to parse.
         * @return Parsed data as QVariantMap for QML.
         */
        QVariantMap _parseFetchedData(const QPair<QString, QList<LocationData>> & data) const;
    private slots:
        /**
         * @brief Handle fetched GeoNames data from provider.
         * @param fetchResult - Fetched GeoNames data result.
         */
        void dataFetchedHandler(const GeoNames::FetchResult &fetchResult);

        /**
         * @brief Handle the api error.
         * @param error - error in text view.
         */
        void handleApiError(const QString &error);
    };
}

#endif // GEO_NAMES_VIEW_MODEL_H
