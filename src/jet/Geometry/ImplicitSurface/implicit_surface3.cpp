#include<jet.h>
#include "implicit_surface3.h"

namespace jet
{
    ImplicitSurface3::ImplicitSurface3(const Transform3& transform, bool IsNormalFlipped)
                        : Surface3(transform, IsNormalFlipped)
    {}

    ImplicitSurface3::ImplicitSurface3(const ImplicitSurface3& other)
                    : Surface3(other)
    {}

    ImplicitSurface3::~ImplicitSurface3()
    {}

    double ImplicitSurface3::SignedDistance(const Vector3D& otherPoint) const
    {
        return SignedDistanceLocal(transform.ToLocal(otherPoint));
    }

    double ImplicitSurface3::ClosestDistanceLocal(const Vector3D& otherPoint) const
    {
        return std::fabs(SignedDistanceLocal(otherPoint));
    }
}