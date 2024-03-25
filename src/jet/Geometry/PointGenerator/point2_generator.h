#pragma once

#include <Arrays/array1.h>
#include <Geometry/BoundingBox/bounding_box2.h>

#include <functional>
#include <memory>

namespace jet
{
    //! \brief Abstract base class for 2D point generator.
    //!
    //! This class provides interface for 2D point generator. For given bounding
    //! box and point spacing, the inherited classes generated points with specified
    //! pattern.
    class PointGenerator2
    {
    public:
        PointGenerator2();

        virtual ~PointGenerator2();

        //! Generates points to output array \p points inside given \p boundingBox
        //! with target point \p spacing
        void Generate(const BoundingBox2D& boundingBox, double spacing,
                    Array1<Vector2D>* points) const;
        
        //! \brief Iterates every point within the bounding box with specified point pattern
        //! and invokes the callback function.
        virtual void ForEachPoint(const BoundingBox2D& boundingBox, double spacing,
                    const std::function<bool(const Vector2D&)>& callback) const = 0;
    };

    typedef std::shared_ptr<PointGenerator2> PointGenerator2Ptr;
}