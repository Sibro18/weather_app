#ifndef GEO_NAMES_FETCH_RESULT_H
#define GEO_NAMES_FETCH_RESULT_H

#include "../request-data/request-data.h"
#include "../geo-parsing-data/geo-parsing-data.h"
#include <QString>
#include <QList>
#include <QPair>
#include <QObject>
#include <QVariantMap>


namespace GeoNames
{
    /**
     * @brief Result of fetching and parsing GeoNames data.
     */
    class FetchResult : public QObject
    {
        Q_OBJECT
    public:
        /**
         * @brief Constructs the FetchResult.
         * @param parent - QObject.
         */
        explicit FetchResult(QObject* parent = nullptr);

        /**
         * @brief Error description if any occurred.
         */
        QString errorString;

        /**
         * @brief Pair of parsed data entries.
         *
         * First = countryCode;
         * Second = GeoParsingData.
         */
        QPair<QString, QList<GeoParsingData>> data;

        /**
         * @brief Original request parameters.
         */
        RequestData requestData;
    };
}
#endif // GEO_NAMES_FETCH_RESULT_H
