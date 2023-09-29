#include<jet.h>
#include"surface_to_implicit3.h"

namespace jet
{
    SurfaceToImplicit3::SurfaceToImplicit3(const Surface3Ptr& surface, const Transform3& transform,
                                            bool IsNormalFlipped)
                                            : ImplicitSurface3(transform, IsNormalFlipped),
                                                _Surface(surface)
    {}

    SurfaceToImplicit3::SurfaceToImplicit3(const SurfaceToImplicit3& other)
                                    : ImplicitSurface3(other),
                                    _Surface(other._Surface)
    {}

    Surface3Ptr SurfaceToImplicit3::Surface() const
    {
        return _Surface;
    }

    Vector3D SurfaceToImplicit3::ClosestPointLocal(const Vector3D& otherPoint) const
    {
        return _Surface->ClosestPoint(otherPoint);
    }

    Vector3D SurfaceToImplicit3::ClosestNormalLocal(const Vector3D& otherPoint) const
    {
        return _Surface->ClosestNormal(otherPoint);
    }

    double SurfaceToImplicit3::ClosestDistanceLocal(const Vector3D& otherPoint) const
    {
        return _Surface->ClosestDistance(otherPoint);
    }

    bool SurfaceToImplicit3::IntersectsLocal(const Ray3D& ray) const
    {
        return _Surface->Intersects(ray);
    }

    SurfaceRayIntersection3 SurfaceToImplicit3::ClosestIntersectionLocal(const Ray3D& ray) const
    {
        return _Surface->ClosestIntersection(ray);
    }

    BoundingBox3D SurfaceToImplicit3::BoundingBoxLocal() const
    {
        return _Surface->BoundingBox();
    }

    double SurfaceToImplicit3::SignedDistanceLocal(const Vector3D& otherPoint) const
    {
        Vector3D x = _Surface->ClosestPoint(otherPoint);
        Vector3D n = _Surface->ClosestNormal(otherPoint);
        n = (IsNormalFlipped) ? -n : n;
        if(n.Dot(otherPoint - x) < 0.0)
        {
            return -x.DistanceTo(otherPoint);
        }
        else
            return x.DistanceTo(otherPoint);
    }

    SurfaceToImplicit3::Builder&
    SurfaceToImplicit3::Builder::WithSurface(const Surface3Ptr& surface)
    {
        _Surface = surface;
        return *this;
    }

    SurfaceToImplicit3 SurfaceToImplicit3::Builder::Build() const
    {
        return SurfaceToImplicit3(_Surface, _transform, _IsNormalFlipped);
    }

    SurfaceToImplicit3Ptr
    SurfaceToImplicit3::Builder::MakeShared() const {
        return std::shared_ptr<SurfaceToImplicit3>(
            new SurfaceToImplicit3(
                _Surface,
                _transform,
                _IsNormalFlipped),
            [] (SurfaceToImplicit3* obj) {
                delete obj;
            });
    }
}