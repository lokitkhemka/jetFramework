#pragma once

#include<Geometry/Surface/surface2.h>

namespace jet
{
    //! Abstract base class for 2D implicit surface.
    class ImplicitSurface2 : public Surface2
    {
    public:
        //! Default Constructor
        ImplicitSurface2(const Transform2& transform = Transform2(), bool IsNormalFlipped = false);

        //! Copy Constructor
        ImplicitSurface2(const ImplicitSurface2& other);

        //! Default Destructor
        virtual ~ImplicitSurface2();

        //! Returns signed distance from the given point \p otherPoint.
        double SignedDistance(const Vector2D& otherPoint) const;
    
    protected:
        //! Returns signed distance from the given point \p otherPoint in local space.
        virtual double SignedDistanceLocal(const Vector2D& otherPoint) const = 0;

    private:
        double ClosestDistanceLocal(const Vector2D& otherPoint) const override;
    };

    typedef std::shared_ptr<ImplicitSurface2> ImplicitSurface2Ptr;
}