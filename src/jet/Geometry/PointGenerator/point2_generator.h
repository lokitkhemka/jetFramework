#pragma once

#include <Arrays/array1.h>
#include <Geometry/BoundingBox/bounding_box2.h>

#include <functional>
#include <memory>

namespace jet
{
    //! \brief Abstract base class for 2D point generation
    //!
    //! This class provides interface for 2D point generation. For given bounding
    //! box and point spacing, the inherited classes generates points with
    //! specified pattern.
    class PointGenerator2
    {
    public:
        PointGenerator2();

        virtual ~PointGenerator2();

        //! Generates point to output array \p points inside given \p BoundingBox
        //! with target point \p spacing.
        void Generate(const BoundingBox2D& boundingBox, double spacing,
                        Array1<Vector2D>* points) const;

        //! \brief Iterates every point within the bounding box with specified
        //! point pattern and invokes the callback function.
        //!
        //! This function iterates every point within the bounding box and invokes the
        //! callback function. The position of the point is specified by the actual implementation.
        //! The suggested spacing between the points are given by \p spacing. The input parameter
        //! of the callback function is the position of the point and the return value tells whether
        //! the iteration should stop or not.
        virtual void ForEachPoint(const BoundingBox2D& boundingBox, double spacing,
                    const std::function<bool(const Vector2D&)>& callback) const = 0;
    };

    typedef std::shared_ptr<PointGenerator2> PointGenerator2Ptr;
}