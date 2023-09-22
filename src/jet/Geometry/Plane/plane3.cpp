#include<jet.h>
#include"plane3.h"

#include<limits>

namespace jet
{
    Plane3::Plane3(const Transform3& transform_, bool IsNormalFlipped_)
            : Surface3(transform_, IsNormalFlipped_)
    {}

    Plane3::Plane3(const Vector3D& normal, const Vector3D& point,
                    const Transform3& transform_, bool IsNormalFlipped_)
                    : Surface3(transform_, IsNormalFlipped_),
                        Normal(normal), Point(point)
    {}

    Plane3::Plane3(const Vector3D& point0, const Vector3D& point1,
                    const Vector3D& point2, const Transform3& transform_, bool IsNormalFlipped_)
                    : Surface3(transform_, IsNormalFlipped_)
    {
        Normal = (point1 - point0).Cross(point2 - point0).Normalized();
        Point = point0;
    }


    Plane3::Plane3(const Plane3& other)
            : Surface3(other), Normal(other.Normal),
                Point(other.Point)
    {}


    Vector3D Plane3::ClosestPointLocal(const Vector3D& otherPoint) const
    {
        Vector3D r = otherPoint - Point;
        return r - Normal.Dot(r) * Normal + Point;
    }

    Vector3D Plane3::ClosestNormalLocal(const Vector3D& otherPoint) const
    {
        UNUSED_VARAIBLE(otherPoint);
        return Normal;
    }

    bool Plane3::IntersectsLocal(const Ray3D& ray) const
    {
        return std::fabs(ray.Direction.Dot(Normal)) > 0;
    }

    SurfaceRayIntersection3 Plane3::ClosestIntersectionLocal(const Ray3D& ray) const
    {
        SurfaceRayIntersection3 intersection;

        double dDotN = ray.Direction.Dot(Normal);

        // Check if not parallel
        if (std::fabs(dDotN) > 0) {
            double t = Normal.Dot(Point - ray.Origin) / dDotN;
            if (t >= 0.0) {
                intersection.IsIntersecting = true;
                intersection.t = t;
                intersection.Point = ray.PointAt(t);
                intersection.Normal = Normal;
            }
        }

        return intersection;
    }

    BoundingBox3D Plane3::BoundingBoxLocal() const
    {
        static const double eps = std::numeric_limits<double>::epsilon();
        static const double dmax = std::numeric_limits<double>::max();

        if (std::fabs(Normal.Dot(Vector3D(1,0,0)) - 1.0) < eps)
        {
            return BoundingBox3D(
                Point - Vector3D(0, dmax, dmax),
                Point + Vector3D(0, dmax, dmax)
            );
        }
        else if (std::fabs(Normal.Dot(Vector3D(0, 1, 0)) - 1.0) < eps) {
            return BoundingBox3D(
                Point - Vector3D(dmax, 0, dmax),
                Point + Vector3D(dmax, 0, dmax));
        } else if (std::fabs(Normal.Dot(Vector3D(0, 0, 1)) - 1.0) < eps) {
            return BoundingBox3D(
                Point - Vector3D(dmax, dmax, 0),
                Point + Vector3D(dmax, dmax, 0));
        } else {
            return BoundingBox3D(
                Vector3D(dmax, dmax, dmax),
                Vector3D(dmax, dmax, dmax));
        }
    }

    Plane3::Builder Plane3::builder()
    {
        return Builder();
    }

    Plane3::Builder& Plane3::Builder::WithNormal(const Vector3D& normal)
    {
        _normal = normal;
        return *this;
    }

    Plane3::Builder& Plane3::Builder::WithPoint(const Vector3D& point)
    {
        _point = point;
        return *this;
    }

    Plane3 Plane3::Builder::Build() const
    {
        return Plane3(_normal, _point, _transform, _IsNormalFlipped);
    }

    Plane3Ptr Plane3::Builder::MakeShared() const
    {
        return std::shared_ptr<Plane3>(
            new Plane3(_normal, _point, _transform, _IsNormalFlipped),
                    [] (Plane3* obj){delete obj;});
    }
}