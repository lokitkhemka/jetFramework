#include <jet.h>
#include "point2_generator.h"

namespace jet
{
    PointGenerator2::PointGenerator2() 
    {}

    PointGenerator2::~PointGenerator2() 
    {}

    void PointGenerator2::Generate(
        const BoundingBox2D& boundingBox,
        double spacing,
        Array1<Vector2D>* points) const 
    {
        ForEachPoint(
            boundingBox,
            spacing,
            [&points](const Vector2D& point) {
                points->Append(point);
                return true;
            });
    }
}