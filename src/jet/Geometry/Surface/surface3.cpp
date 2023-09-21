#include<jet.h>
#include"surface3.h"
#include<Geometry/Transform/transform3.h>
#include<algorithm>

namespace jet
{
    Surface3::Surface3(const Transform3& transform_, bool isNormalFlipped_)
    : transform(transform_)
    , IsNormalFlipped(isNormalFlipped_) {
    }

    Surface3::Surface3(const Surface3& other)
    : transform(other.transform)
    , IsNormalFlipped(other.IsNormalFlipped) {
    }

    Surface3::~Surface3() {
    }

    Vector3D Surface3::ClosestPoint(const Vector3D& otherPoint) const {
        return transform.ToWorld(ClosestPointLocal(transform.ToLocal(otherPoint)));
    }

    BoundingBox3D Surface3::BoundingBox() const {
        return transform.ToWorld(BoundingBoxLocal());
    }

    bool Surface3::Intersects(const Ray3D& ray) const {
        return IntersectsLocal(transform.ToLocal(ray));
    }

    double Surface3::ClosestDistance(const Vector3D& otherPoint) const {
        return ClosestDistanceLocal(transform.ToLocal(otherPoint));
    }

    SurfaceRayIntersection3 Surface3::ClosestIntersection(const Ray3D& ray) const {
        auto result = ClosestIntersectionLocal(transform.ToLocal(ray));
        result.Point = transform.ToWorld(result.Point);
        result.Normal = transform.ToWorldDirection(result.Normal);
        result.Normal *= (IsNormalFlipped) ? -1.0 : 1.0;
        return result;
    }

    Vector3D Surface3::ClosestNormal(const Vector3D& otherPoint) const {
        auto result = transform.ToWorldDirection(
            ClosestNormalLocal(transform.ToLocal(otherPoint)));
        result *= (IsNormalFlipped) ? -1.0 : 1.0;
        return result;
    }

    bool Surface3::IntersectsLocal(const Ray3D& rayLocal) const {
        auto result = ClosestIntersectionLocal(rayLocal);
        return result.IsIntersecting;
    }

    double Surface3::ClosestDistanceLocal(const Vector3D& otherPointLocal) const {
        return otherPointLocal.DistanceTo(ClosestPointLocal(otherPointLocal));
    }
}