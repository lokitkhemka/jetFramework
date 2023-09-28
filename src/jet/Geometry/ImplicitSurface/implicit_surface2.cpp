#include<jet.h>
#include "implicit_surface2.h"

namespace jet
{
    ImplicitSurface2::ImplicitSurface2(const Transform2& transform, bool IsNormalFlipped)
                        : Surface2(transform, IsNormalFlipped)
    {}

    ImplicitSurface2::ImplicitSurface2(const ImplicitSurface2& other)
                    : Surface2(other)
    {}

    ImplicitSurface2::~ImplicitSurface2()
    {}

    double ImplicitSurface2::SignedDistance(const Vector2D& otherPoint) const
    {
        return SignedDistanceLocal(transform.ToLocal(otherPoint));
    }

    double ImplicitSurface2::ClosestDistanceLocal(const Vector2D& otherPoint) const
    {
        return std::fabs(SignedDistanceLocal(otherPoint));
    }
}