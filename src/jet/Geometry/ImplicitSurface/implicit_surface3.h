#pragma once

#include<Geometry/Surface/surface3.h>

namespace jet
{
    //! Abstract base class for 3D implicit surface.
    class ImplicitSurface3 : public Surface3
    {
    public:
        //! Default Constructor
        ImplicitSurface3(const Transform3& transform = Transform3(), bool IsNormalFlipped = false);

        //! Copy Constructor
        ImplicitSurface3(const ImplicitSurface3& other);

        //! Default Destructor
        virtual ~ImplicitSurface3();

        //! Returns signed distance from the given point \p otherPoint.
        double SignedDistance(const Vector3D& otherPoint) const;
    
    protected:
        //! Returns signed distance from the given point \p otherPoint in local space.
        virtual double SignedDistanceLocal(const Vector3D& otherPoint) const = 0;

    private:
        double ClosestDistanceLocal(const Vector3D& otherPoint) const override;
    };

    typedef std::shared_ptr<ImplicitSurface3> ImplicitSurface3Ptr;
}