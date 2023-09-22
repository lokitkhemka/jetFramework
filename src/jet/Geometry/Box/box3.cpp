#include<jet.h>
#include"box3.h"
#include<Geometry/Plane/plane3.h>

namespace jet
{
    Box3::Box3(const Transform3& transform, bool IsNormalFlipped)
                : Surface3(transform, IsNormalFlipped)
                {}


    Box3::Box3(const Vector3D& lowerCorner, const Vector3D& upperCorner, const Transform3& transform,
                bool IsNormalFlipped)
                : Box3(BoundingBox3D(lowerCorner, upperCorner), transform, IsNormalFlipped)
    {}
    
    
    Box3::Box3(const BoundingBox3D& boundingBox, const Transform3& transform,
                    bool IsNormalFlipped)
                    : Surface3(transform, IsNormalFlipped), Bound(boundingBox)
    {}

    Box3::Box3(const Box3& other)
        : Surface3(other), Bound(other.Bound)
    {}

    Vector3D Box3::ClosestPointLocal(const Vector3D& otherPoint) const {
        if (Bound.Contains(otherPoint)) {
            Plane3 planes[6] = {
                Plane3(Vector3D(1, 0, 0), Bound.UpperCorner),
                Plane3(Vector3D(0, 1, 0), Bound.UpperCorner),
                Plane3(Vector3D(0, 0, 1), Bound.UpperCorner),
                Plane3(Vector3D(-1, 0, 0), Bound.LowerCorner),
                Plane3(Vector3D(0, -1, 0), Bound.LowerCorner),
                Plane3(Vector3D(0, 0, -1), Bound.LowerCorner)
            };

            Vector3D result = planes[0].ClosestPoint(otherPoint);
            double distanceSquared = result.DistanceSquaredTo(otherPoint);

            for (int i = 1; i < 6; ++i) {
                Vector3D localResult = planes[i].ClosestPoint(otherPoint);
                double localDistanceSquared
                    = localResult.DistanceSquaredTo(otherPoint);

                if (localDistanceSquared < distanceSquared) {
                    result = localResult;
                    distanceSquared = localDistanceSquared;
                }
            }

            return result;
        } else {
            return Clamp(
                otherPoint,
                Bound.LowerCorner,
                Bound.UpperCorner);
        }
    }

    Vector3D Box3::ClosestNormalLocal(const Vector3D& otherPoint) const {
        Plane3 planes[6] = {
            Plane3(Vector3D(1, 0, 0), Bound.UpperCorner),
            Plane3(Vector3D(0, 1, 0), Bound.UpperCorner),
            Plane3(Vector3D(0, 0, 1), Bound.UpperCorner),
            Plane3(Vector3D(-1, 0, 0), Bound.LowerCorner),
            Plane3(Vector3D(0, -1, 0), Bound.LowerCorner),
            Plane3(Vector3D(0, 0, -1), Bound.LowerCorner)
        };

        if (Bound.Contains(otherPoint)) {
            Vector3D closestNormal = planes[0].Normal;
            Vector3D closestPoint = planes[0].ClosestPoint(otherPoint);
            double minDistanceSquared = (closestPoint - otherPoint).LengthSquared();

            for (int i = 1; i < 6; ++i) {
                Vector3D localClosestPoint = planes[i].ClosestPoint(otherPoint);
                double localDistanceSquared
                    = (localClosestPoint - otherPoint).LengthSquared();

                if (localDistanceSquared < minDistanceSquared) {
                    closestNormal = planes[i].Normal;
                    minDistanceSquared = localDistanceSquared;
                }
            }

            return closestNormal;
        } else {
            Vector3D closestPoint = Clamp(
                otherPoint,
                Bound.LowerCorner,
                Bound.UpperCorner);
            Vector3D closestPointToInputPoint = otherPoint - closestPoint;
            Vector3D closestNormal = planes[0].Normal;
            double maxCosineAngle = closestNormal.Dot(closestPointToInputPoint);

            for (int i = 1; i < 6; ++i) {
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


    bool Box3::IntersectsLocal(const Ray3D& ray) const
    {
        return Bound.Intersects(ray);
    }

    SurfaceRayIntersection3 Box3::ClosestIntersectionLocal(const Ray3D& ray) const
    {
        SurfaceRayIntersection3 intersection;
        BoundingBoxRayIntersection3D bbRayIntersection = Bound.ClosestIntersection(ray);

        intersection.IsIntersecting = bbRayIntersection.IsIntersecting;

        if(intersection.IsIntersecting)
        {
            intersection.t = bbRayIntersection.tNear;
            intersection.Point = ray.PointAt(bbRayIntersection.tNear);
            intersection.Normal = Box3::ClosestNormal(intersection.Point);
        }

        return intersection;
    }

    BoundingBox3D Box3::BoundingBoxLocal() const {
        return Bound;
    }

    Box3::Builder Box3::builder() {
        return Builder();
    }

    Box3::Builder& Box3::Builder::WithLowerCorner(const Vector3D& pt) {
        _lowerCorner = pt;
        return *this;
    }

    Box3::Builder& Box3::Builder::WithUpperCorner(const Vector3D& pt) {
        _upperCorner = pt;
        return *this;
    }

    Box3::Builder& Box3::Builder::WithBoundingBox(const BoundingBox3D& bbox) {
        _lowerCorner = bbox.LowerCorner;
        _upperCorner = bbox.UpperCorner;
        return *this;
    }

    Box3 Box3::Builder::Build() const {
        return Box3(_lowerCorner, _upperCorner, _transform, _IsNormalFlipped);
    }

    Box3Ptr Box3::Builder::MakeShared() const {
        return std::shared_ptr<Box3>(
            new Box3(
                _lowerCorner,
                _upperCorner,
                _transform,
                _IsNormalFlipped),
            [] (Box3* obj) {
                delete obj;
            });
    }
}