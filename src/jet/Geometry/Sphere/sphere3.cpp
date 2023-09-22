#include<jet.h>
#include "sphere3.h"

#include <limits>

namespace jet
{
    Sphere3::Sphere3(const Transform3& transform_, bool IsNormalFlipped_)
        : Surface3(transform_, IsNormalFlipped_)
    {}

    Sphere3::Sphere3(const Vector3D& center_, double radius_,
                    const Transform3& transform_, bool IsNormalFlipped_)
                    : Surface3(transform_, IsNormalFlipped_),
                        Center(center_), Radius(radius_)
    {}

    Sphere3::Sphere3(const Sphere3& other)
        : Surface3(other), Center(other.Center),
            Radius(other.Radius)
    {}

    Vector3D Sphere3::ClosestPointLocal(const Vector3D& otherPoint) const
    {
        return Radius * ClosestNormalLocal(otherPoint) + Center;
    }

    double Sphere3::ClosestDistanceLocal(const Vector3D& otherPoint) const
    {
        return std::fabs(Center.DistanceTo(otherPoint) - Radius);
    }

    Vector3D Sphere3::ClosestNormalLocal(const Vector3D& otherPoint) const
    {
        if (Center.IsSimilar(otherPoint))
        {
            return Vector3D(1,0,0);
        }
        else
        {
            return (otherPoint - Center).Normalized();
        }
    }

    bool Sphere3::IntersectsLocal(const Ray3D& ray) const
    {
        Vector3D r = ray.Origin - Center;
        double b = ray.Direction.Dot(r);
        double c = r.LengthSquared() - Square(Radius);
        double d = b * b - c;

        if (d > 0.)
        {
            d = std::sqrt(d);
            double tMin = -b - d;
            double tMax = -b + d;

            if (tMin < 0.0)
            {
                tMin = tMax;
                tMax = std::numeric_limits<double>::max();
            }

            if ( tMin >= 0.0)
            {
                return true;
            }
        }
        return false;
    }

    SurfaceRayIntersection3 Sphere3::ClosestIntersectionLocal(const Ray3D& ray) const
    {
        SurfaceRayIntersection3 intersection;
        Vector3D r = ray.Origin - Center;
        double b = ray.Direction.Dot(r);
        double c = r.LengthSquared() - Square(Radius);
        double d = b * b - c;

        if (d > 0.)
        {
            d = std::sqrt(d);
            double tMin = -b - d;
            double tMax = -b + d;

            if (tMin < 0.0)
            {
                tMin = tMax;
                tMax = std::numeric_limits<double>::max();
            }

            if ( tMin < 0.0)
            {
                intersection.IsIntersecting = false;
            }
            else
            {
                intersection.IsIntersecting = true;
                intersection.t = tMin;
                intersection.Point = ray.Origin + tMin * ray.Direction;
                intersection.Normal = (intersection.Point - Center).Normalized();
            }
        }
        else
        {
            intersection.IsIntersecting = false;
        }
        return intersection;
    }

    BoundingBox3D Sphere3::BoundingBoxLocal() const {
        Vector3D r(Radius, Radius, Radius);
        return BoundingBox3D(Center - r, Center + r);
    }

    Sphere3::Builder Sphere3::builder() {
        return Builder();
    }

    Sphere3::Builder& Sphere3::Builder::WithCenter(const Vector3D& center) {
        _center = center;
        return *this;
    }

    Sphere3::Builder& Sphere3::Builder::WithRadius(double radius) {
        _radius = radius;
        return *this;
    }

    Sphere3 Sphere3::Builder::Build() const {
        return Sphere3(_center, _radius, _transform, _IsNormalFlipped);
    }

    Sphere3Ptr Sphere3::Builder::MakeShared() const {
        return std::shared_ptr<Sphere3>(
            new Sphere3(
                _center,
                _radius,
                _transform,
                _IsNormalFlipped),
            [] (Sphere3* obj) {
                delete obj;
        });
    }
}