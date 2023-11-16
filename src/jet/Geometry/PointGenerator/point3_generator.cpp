#include <jet.h>
#include "point3_generator.h"

namespace jet
{
    PointGenerator3::PointGenerator3() 
    {}

    PointGenerator3::~PointGenerator3() 
    {}

    void PointGenerator3::Generate(
        const BoundingBox3D& boundingBox,
        double spacing,
        Array1<Vector3D>* points) const 
    {
        ForEachPoint(
            boundingBox,
            spacing,
            [&points](const Vector3D& point) {
                points->Append(point);
                return true;
            });
    }
}