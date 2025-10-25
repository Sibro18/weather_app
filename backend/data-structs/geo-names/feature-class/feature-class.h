#ifndef FEATURE_CLASS_H
#define FEATURE_CLASS_H

#include <QString>


namespace GeoNames
{
    /**
     * @brief Wrapper for the GeoNames API featureClass code.
     */
    class FeatureClass
    {
    public:
        /**
         * @brief Enumeration of available GeoNames feature classes.
         */
        enum Code : char
        {
            Admin      = 'A', ///< Administrative region, country, state, region
            Hydro      = 'H', ///< Hydrographic features
            Locality   = 'L', ///< Parks, areas, localities
            Populated  = 'P', ///< Populated places
            RoadRail   = 'R', ///< Roads, railroads, transport lines
            Structure  = 'S', ///< Structures (buildings, facilities)
            Terrain    = 'T', ///< Terrain (mountains, hills, volcanoes, etc.)
            Undersea   = 'U', ///< Undersea features
            Vegetation = 'V'  ///< Vegetation (forests, etc.)
        };

        /**
         * @brief Construtcs a feature class with the specified code.
         * @param code - The Feature class code.
         */
        explicit FeatureClass(FeatureClass::Code code);

        /**
         * @brief Get the code value in QString.
         * @return Code as a QString (e.g., "P", "A").
         */
        QString toString() const;

        /**
         * @brief Get the code value.
         * @return The stored feature class code.
         */
        Code code() const;

        /**
         * @brief Get FeatureClass from its string representation.
         * @param s - String containing the code.
         * @return A FeatureClass object.
         */
        static FeatureClass fromString(const QString &s);

    private:
        /**
         * @brief Stored feature class code.
         */
        Code _code;
    };
}

#endif // FEATURE_CLASS_H
