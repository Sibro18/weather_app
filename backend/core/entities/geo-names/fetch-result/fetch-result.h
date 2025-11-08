#ifndef GEO_NAMES_FETCH_RESULT_H
#define GEO_NAMES_FETCH_RESULT_H


#include "../request-data/request-data.h"
#include "../location-data/location-data.h"


namespace GeoNames
{
    /**
     * @brief Result of fetching and parsing GeoNames data.
     */
    class FetchResult
    {
    public:
        /**
         * @brief Default constructor.
         */
        FetchResult() = default;

        /**
         * @brief Pair of parsed data entries.
         *
         * First = countryCode;
         * Second = List<LocationData>.
         */
        QPair<QString, QList<LocationData>> data;

        /**
         * @brief Request parameters.
         */
        RequestData requestData;
    };
}
#endif // GEO_NAMES_FETCH_RESULT_H
