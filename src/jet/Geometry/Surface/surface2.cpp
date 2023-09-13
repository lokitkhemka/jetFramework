#include <jet.h>
#include "surface2.h"
#include<algorithm>

namespace jet
{
    Surface2::Surface2(const Transform2& transform_, bool IsNormalFlipped_)
        : transform(transform_), IsNormalFlipped(IsNormalFlipped_)
    {}
    
    Surface2::Surface2(const Surface2& other)
        :transform(other.transform), IsNormalFlipped(other.IsNormalFlipped)
    {}

    Surface2::~Surface2()
    {}

    Vector2D Surface2::ClosestPoint(const Vector2D& otherPoint) const
    {
        return transform.ToWorld(ClosestPointLocal(transform.ToLocal(otherPoint)));
    }

    BoundingBox2D Surface2::BoundingBox() const
    {
        return transform.ToWorld(BoundingBoxLocal());
    }

    bool Surface2::Intersects(const Ray2D& ray) const
    {
        return IntersectsLocal(transform.ToLocal(ray));
    }

    double Surface2::ClosestDistance(const Vector2D& otherPoint) const
    {
        return ClosestDistanceLocal(transform.ToLocal(otherPoint));
    }

    SurfaceRayIntersection2 Surface2::ClosestIntersection(const Ray2D& ray) const
    {
        auto result = ClosestIntersectionLocal(transform.ToLocal(ray));

        result.Point = transform.ToWorld(result.Point);
        result.Normal = transform.ToWorldDirection(result.Normal);
        result.Normal *= (IsNormalFlipped) ? -1.0 : 1.0;
        return result;
    }

    Vector2D Surface2::ClosestNormal(const Vector2D& otherPoint) const
    {
        auto result = transform.ToWorldDirection(ClosestNormalLocal(transform.ToLocal(otherPoint)));
        result *= (IsNormalFlipped) ? -1.0 : 1.0;
        return result;
    }

    bool Surface2::IntersectsLocal(const Ray2D& rayLocal) const
    {
        auto result = ClosestIntersectionLocal(rayLocal);
        return result.IsIntersecting;
    }

    double Surface2::ClosestDistanceLocal(const Vector2D& otherPointLocal) const
    {
        return otherPointLocal.DistanceTo(ClosestPointLocal(otherPointLocal));
    }
}