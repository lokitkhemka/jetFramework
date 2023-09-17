#include<jet.h>
#include "box2.h"
#include <Geometry/Plane/plane2.h>
#include <math-utils.h>

namespace jet
{
    Box2::Box2(
        const Transform2& transform,
        bool isNormalFlipped)
    : Surface2(transform, isNormalFlipped)
            {}
    

    Box2::Box2(const Vector2D& lowerCorner, const Vector2D& upperCorner,
                const Transform2& transform, bool isNormalFlipped) :
                Box2(BoundingBox2D(lowerCorner, upperCorner), transform, isNormalFlipped)
    {}


    Box2::Box2(const BoundingBox2D& boundingBox, const Transform2& transform, bool isNormalFlipped):
                Surface2(transform, isNormalFlipped), Bound(boundingBox)
    {}

    Box2::Box2(const Box2& other) : Surface2(other), Bound(other.Bound)
    {}

    Vector2D Box2::ClosestPointLocal(const Vector2D& otherPoint) const
    {
        if (Bound.Contains(otherPoint))
        {
            Plane2 planes[4] = {Plane2(Vector2D(1,0), Bound.UpperCorner),
                                Plane2(Vector2D(0,1), Bound.UpperCorner),
                                Plane2(Vector2D(-1,0), Bound.LowerCorner),
                                Plane2(Vector2D(0,-1), Bound.LowerCorner)};
            
            Vector2D result = planes[0].ClosestPoint(otherPoint);
            double DistanceSquared = result.DistanceSquaredTo(otherPoint);

            for(int i = 1; i < 4; ++i)
            {
                Vector2D localResult = planes[i].ClosestPoint(otherPoint);
                double localDistanceSquared = localResult.DistanceSquaredTo(otherPoint);

                if(localDistanceSquared < DistanceSquared)
                {
                    result = localResult;
                    DistanceSquared = localDistanceSquared;
                }
            }

            return result;
        }
        else
        {
            return Clamp(otherPoint, Bound.LowerCorner, Bound.UpperCorner);
        }
    }

    Vector2D Box2::ClosestNormalLocal(const Vector2D& otherPoint) const {
        Plane2 planes[4] = {
            Plane2(Vector2D(1, 0), Bound.UpperCorner),
            Plane2(Vector2D(0, 1), Bound.UpperCorner),
            Plane2(Vector2D(-1, 0), Bound.LowerCorner),
            Plane2(Vector2D(0, -1), Bound.LowerCorner)
        };

        if (Bound.Contains(otherPoint)) {
            Vector2D closestNormal = planes[0].Normal;
            Vector2D closestPoint = planes[0].ClosestPoint(otherPoint);
            double minDistanceSquared = (closestPoint - otherPoint).LengthSquared();

            for (int i = 1; i < 4; ++i) {
                Vector2D localClosestPoint = planes[i].ClosestPoint(otherPoint);
                double localDistanceSquared
                    = (localClosestPoint - otherPoint).LengthSquared();

                if (localDistanceSquared < minDistanceSquared) {
                    closestNormal = planes[i].Normal;
                    minDistanceSquared = localDistanceSquared;
                }
            }

            return closestNormal;
        } else {
            Vector2D closestPoint = Clamp(
                otherPoint,
                Bound.LowerCorner,
                Bound.UpperCorner);
            Vector2D closestPointToInputPoint = otherPoint - closestPoint;
            Vector2D closestNormal = planes[0].Normal;
            double maxCosineAngle = closestNormal.Dot(closestPointToInputPoint);

            for (int i = 1; i < 4; ++i) {
                double cosineAngle
                    = planes[i].Normal.Dot(closestPointToInputPoint);

                if (cosineAngle > maxCosineAngle) {
                    closestNormal = planes[i].Normal;
                    maxCosineAngle = cosineAngle;
                }
            }

            return closestNormal;
        }
    }

    bool Box2::IntersectsLocal(const Ray2D& ray) const {
        return Bound.Intersects(ray);
    }

    SurfaceRayIntersection2 Box2::ClosestIntersectionLocal(
        const Ray2D& ray) const {
        SurfaceRayIntersection2 intersection;
        BoundingBoxRayIntersection2D bbRayIntersection
            = Bound.ClosestIntersection(ray);
        intersection.IsIntersecting = bbRayIntersection.IsIntersecting;
        if (intersection.IsIntersecting) {
            intersection.t = bbRayIntersection.tNear;
            intersection.Point = ray.PointAt(bbRayIntersection.tNear);
            intersection.Normal = Box2::ClosestNormal(intersection.Point);
        }
        return intersection;
    }

    BoundingBox2D Box2::BoundingBoxLocal() const
    {
        return Bound;
    }

    Box2::Builder Box2::builder()
    {
        return Builder();
    }

    Box2::Builder& Box2::Builder::WithLowerCorner(const Vector2D& pt)
    {
        _lowerCorner = pt;
        return *this;
    }

    Box2::Builder& Box2::Builder::WithUpperCorner(const Vector2D& pt)
    {
        _upperCorner = pt;
        return *this;
    }

    Box2::Builder& Box2::Builder::WithBoundingBox(const BoundingBox2D& bbox)
    {
        _lowerCorner = bbox.LowerCorner;
        _upperCorner = bbox.UpperCorner;
        return *this;
    }

    Box2 Box2::Builder::Build() const
    {
        return Box2(_lowerCorner, _upperCorner, _transform, _IsNormalFlipped);
    }

    Box2Ptr Box2::Builder::MakeShared() const {
        return std::shared_ptr<Box2>(
            new Box2(
                _lowerCorner,
                _upperCorner,
                _transform,
                _IsNormalFlipped),
            [] (Box2* obj) {
                delete obj;
            });
    }
}