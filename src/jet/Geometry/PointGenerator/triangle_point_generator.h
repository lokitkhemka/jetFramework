#pragma once

#include <Geometry/PointGenerator/point2_generator.h>

namespace jet
{
    //! \brief Right Triangle Point generator.
    class TrianglePointGenerator final : public PointGenerator2
    {
    public:
        //! \brief Invokes \p callback function for each right triangle points inside
        //! \p boundingBox

        void ForEachPoint(const BoundingBox2D& boundingBox, double spacing,
                            const std::function<bool(const Vector2D&)>& callback) const override;
    };

    typedef std::shared_ptr<TrianglePointGenerator> TrianglePointGeneratorPtr;
}