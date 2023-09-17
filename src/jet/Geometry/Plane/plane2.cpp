#include<jet.h>
#include"plane2.h"
#include<limits>

namespace jet
{
    Plane2::Plane2(const Transform2& transform_, bool IsNormalFlipped_)
    : Surface2(transform_, IsNormalFlipped_)
    {}


    Plane2::Plane2(const Vector2D& normal_, const Vector2D& point_,
                    const Transform2& transform_, bool IsNormalFlipped_)
                    : Surface2(transform_, IsNormalFlipped_), Normal(normal_),
                        Point(point_)
    {}

    Plane2::Plane2(const Plane2& other)
                : Surface2(other),
                    Normal(other.Normal),
                    Point(other.Point)
    {}

    Vector2D Plane2::ClosestPointLocal(const Vector2D& otherPoint) const
    {
        Vector2D r = otherPoint - Point;
        return r - Normal.Dot(r) * Normal + Point;
    }

    Vector2D Plane2::ClosestNormalLocal(const Vector2D& otherPoint) const
    {
        UNUSED_VARAIBLE(otherPoint);
        return Normal;
    }

    bool Plane2::IntersectsLocal(const Ray2D& ray) const
    {
        return std::fabs(ray.Direction.Dot(Normal)) > 0;
    }

    SurfaceRayIntersection2 Plane2::ClosestIntersectionLocal(const Ray2D& ray) const
    {
        SurfaceRayIntersection2 intersection;
        double dDotN = ray.Direction.Dot(Normal);

        //Check if not parallel
        if (std::fabs(dDotN) > 0)
        {
            double t = Normal.Dot(Point - ray.Origin) / dDotN;
            if(t>=0.0)
            {
                intersection.IsIntersecting = true;
                intersection.t = t;
                intersection.Point = ray.PointAt(t);
                intersection.Normal = Normal;
            }
        }
        return intersection;
    }

    BoundingBox2D Plane2::BoundingBoxLocal() const
    {
        if (std::fabs(Normal.Dot(Vector2D(1, 0)) - 1.0) < kEpsilonD)
        {
            return BoundingBox2D(Point - Vector2D(0, kMaxD), Point + Vector2D(0, kMaxD));
        }
        else if (std::fabs(Normal.Dot(Vector2D(0, 1)) - 1.0) < kEpsilonD)
        {
            return BoundingBox2D(Point - Vector2D(kMaxD, 0), Point + Vector2D(kMaxD, 0));
        }
        else
        {
            return BoundingBox2D(Vector2D(kMaxD, kMaxD), Vector2D(kMaxD, kMaxD));
        }
    }

    Plane2::Builder Plane2::builder()
    {
        return Builder();
    }

    Plane2::Builder& Plane2::Builder::WithNormal(const Vector2D& normal)
    {
        _normal = normal;
        return *this;
    }

    Plane2::Builder& Plane2::Builder::WithPoint(const Vector2D& point)
    {
        _point = point;
        return *this;
    }

    Plane2 Plane2::Builder::Build() const
    {
        return Plane2(_normal, _point, _transform, _IsNormalFlipped);
    }

    Plane2Ptr Plane2::Builder::MakeShared() const
    {
        return std::shared_ptr<Plane2>(new Plane2(_normal, _point, _transform, _IsNormalFlipped), 
                        [](Plane2* obj){
                            delete obj;
                        });
    }

    


}