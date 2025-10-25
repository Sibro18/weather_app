#include <QChar>

#include "feature-class.h"


namespace GeoNames
{
    FeatureClass::FeatureClass(Code code)
        : _code(code)
    {}

    QString FeatureClass::toString() const
    {
        return QString(QChar(static_cast<char>(_code)));
    }

    FeatureClass::Code FeatureClass::code() const
    {
        return _code;
    }

    FeatureClass FeatureClass::fromString(const QString &s)
    {
        if (s.size() != 1)
            throw std::invalid_argument("Invalid feature class code");

        switch (s.at(0).toLatin1())
        {
            case 'A': return FeatureClass(Admin);
            case 'H': return FeatureClass(Hydro);
            case 'L': return FeatureClass(Locality);
            case 'P': return FeatureClass(Populated);
            case 'R': return FeatureClass(RoadRail);
            case 'S': return FeatureClass(Structure);
            case 'T': return FeatureClass(Terrain);
            case 'U': return FeatureClass(Undersea);
            case 'V': return FeatureClass(Vegetation);
            default: throw std::invalid_argument("Unknown feature class code");
        }
    }
}
