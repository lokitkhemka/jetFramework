#include<jet.h>
#include"surface_to_implicit2.h"

namespace jet
{
    SurfaceToImplicit2::SurfaceToImplicit2(const Surface2Ptr& surface, const Transform2& transform,
                                            bool IsNormalFlipped)
                                            : ImplicitSurface2(transform, IsNormalFlipped),
                                                _Surface(surface)
    {}

    SurfaceToImplicit2::SurfaceToImplicit2(const SurfaceToImplicit2& other) :
                ImplicitSurface2(other), _Surface(other._Surface)
    {}

    Surface2Ptr SurfaceToImplicit2::Surface() const
    {
        return _Surface;
    }

    Vector2D SurfaceToImplicit2::ClosestPointLocal(const Vector2D& otherPoint) const
    {
        return _Surface->ClosestPoint(otherPoint);
    }

    Vector2D SurfaceToImplicit2::ClosestNormalLocal(const Vector2D& otherPoint) const
    {
        return _Surface->ClosestNormal(otherPoint);
    }

    double SurfaceToImplicit2::ClosestDistanceLocal(const Vector2D& otherPoint) const
    {
        return _Surface->ClosestDistance(otherPoint);
    }

    bool SurfaceToImplicit2::IntersectsLocal( const Ray2D& ray) const
    {
        return _Surface->Intersects(ray);
    }

    SurfaceRayIntersection2 SurfaceToImplicit2::ClosestIntersectionLocal(const Ray2D& ray) const
    {
        return _Surface->ClosestIntersection(ray);
    }

    BoundingBox2D SurfaceToImplicit2::BoundingBoxLocal() const
    {
        return _Surface->BoundingBox();
    }

    double SurfaceToImplicit2::SignedDistanceLocal(const Vector2D& otherPoint) const
    {
        Vector2D x = _Surface->ClosestPoint(otherPoint);
        Vector2D n = _Surface->ClosestNormal(otherPoint);
        n = (IsNormalFlipped) ? -n : n;
        if (n.Dot(otherPoint - x) < 0.0)
            return -x.DistanceTo(otherPoint);
        else
            return x.DistanceTo(otherPoint);
    }

    SurfaceToImplicit2::Builder&
    SurfaceToImplicit2::Builder::WithSurface(const Surface2Ptr& surface)
    {
        _Surface = surface;
        return *this;
    }

    SurfaceToImplicit2 SurfaceToImplicit2::Builder::Build() const
    {
        return SurfaceToImplicit2(_Surface, _transform, _IsNormalFlipped);
    }

    SurfaceToImplicit2Ptr SurfaceToImplicit2::Builder::MakeShared() const
    {
        return std::shared_ptr<SurfaceToImplicit2>(
            new SurfaceToImplicit2(_Surface, _transform, _IsNormalFlipped),
            [](SurfaceToImplicit2* obj)
            {
                delete obj;
            });
    }
}