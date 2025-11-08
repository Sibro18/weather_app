#ifndef GEO_NAMES_REQUEST_DATA_H
#define GEO_NAMES_REQUEST_DATA_H


#include "../feature-class/feature-class.h"


namespace GeoNames
{
    /**
     * @brief Parameters for making a request to the GeoNames API.
     */
    struct RequestData
    {
        /**
         * @brief Optional filter by feature class.
         */
        std::optional<FeatureClass> featureClass;

        /**
         * @brief Name filter for the object.
         */
        QString name = "";

        /**
         * @brief Country code (ISO Alpha-2).
         */
        QString countryCode = "";

        /**
         * @brief Whether to search by name prefix.
         */
        bool startWith = false;

        /**
         * @brief Language code for the response (e.g., "en", "ru").
         */
        QString lang = "";

        /**
         * @brief Get request data from json view.
         * @param requestData - json view.
         * @return RequestData.
         */
        static RequestData createFromQVariantMap(const QVariantMap &requestData);

        /**
         * @brief Convert to json view.
         * @return Json view.
         */
        QVariantMap toVariantMap() const;

        bool operator==(const RequestData& other) const;
        bool operator<(const RequestData& other) const;
    };
}

#endif // GEO_NAMES_REQUEST_DATA_H
