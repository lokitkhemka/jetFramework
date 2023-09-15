#include "sphere2.h"
#include<jet.h>
#include<limits>

namespace jet
{
    Sphere2::Sphere2(const Transform2& transform_, bool IsNormalFlipped_)
        : Surface2(transform_, IsNormalFlipped_)
    {}

    
    Sphere2::Sphere2(const Vector2D& center_, double radius_,
                    const Transform2& transform_, bool IsNormalFlipped_)
                    : Surface2(transform_, IsNormalFlipped_), Center(center_),
                    Radius(radius_)
    {}

    
    Sphere2::Sphere2(const Sphere2& other)
        : Surface2(other), Center(other.Center),Radius(other.Radius)
    {}


    Vector2D Sphere2::ClosestPointLocal(const Vector2D& otherPoint) const
    {
        return Radius * ClosestNormalLocal(otherPoint) + Center;
    }

    double Sphere2::ClosestDistanceLocal(const Vector2D& otherPoint) const
    {
        return std::fabs(Center.DistanceTo(otherPoint) - Radius);
    }

    Vector2D Sphere2::ClosestNormalLocal(const Vector2D& otherPoint) const
    {
        if (Center.IsSimilar(otherPoint)) // If the otherPoint is the center of the circle
        {
            return Vector2D(1,0);
        }
        else
        {
            return (otherPoint - Center).Normalized();
        }
    }

    bool Sphere2::IntersectsLocal(const Ray2D& ray) const
    {
        Vector2D r = ray.Origin - Center;
        double b = ray.Direction.Dot(r);
        double c = r.LengthSquared() - Square(Radius);
        double d = b*b - c;

        if (d > 0.0)
        {
            d = std::sqrt(d);
            double tMin = -b - d;
            double tMax = -b + d;

            if(tMin < 0.0)
            {
                tMin = tMax;
                tMax = kMaxD;
            }

            if(tMin >= 0.0)
            {
                return true;
            }
        }
        return false;
    }

    SurfaceRayIntersection2 Sphere2::ClosestIntersectionLocal(const Ray2D& ray) const
    {
        SurfaceRayIntersection2 intersection;
        Vector2D r = ray.Origin - Center;
        double b = ray.Direction.Dot(r);
        double c = r.LengthSquared() - Square(Radius);
        double d = b*b - c;

        if (d > 0.0)
        {
            d = std::sqrt(d);
            double tMin = -b - d;
            double tMax = -b + d;

            if(tMin < 0.0)
            {
                tMin = tMax;
                tMax = kMaxD;
            }

            if(tMin < 0.0)
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

    BoundingBox2D Sphere2::BoundingBoxLocal() const
    {
        Vector2D r(Radius, Radius);
        return BoundingBox2D(Center - r, Center + r);
    }

    Sphere2::Builder Sphere2::builder()
    {
        return Builder();
    }

    Sphere2::Builder& Sphere2::Builder::WithCenter(const Vector2D& center)
    {
        _center = center;
        return *this;
    }

    Sphere2::Builder& Sphere2::Builder::WithRadius(double radius)
    {
        _radius = radius;
        return *this;
    }

    Sphere2 Sphere2::Builder::Build() const
    {
        return Sphere2(_center, _radius, _transform, _IsNormalFlipped);
    }

    Sphere2Ptr Sphere2::Builder::MakeShared() const
    {
        return std::shared_ptr<Sphere2>(new Sphere2(_center, _radius, _transform, _IsNormalFlipped),
                            [](Sphere2* obj){delete obj;});
    }
}
