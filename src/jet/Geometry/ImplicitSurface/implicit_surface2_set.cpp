#include <jet.h>

#include "implicit_surface2_set.h"
#include <Geometry/Surface/surface_to_implicit2.h>

#include <algorithm>
#include <limits>
#include <vector>

namespace jet
{
    ImplicitSurfaceSet2::ImplicitSurfaceSet2()
    {}

    ImplicitSurfaceSet2::ImplicitSurfaceSet2(const std::vector<ImplicitSurface2Ptr>& surfaces,
                const Transform2& transform, bool IsNormalFlipped)
            : ImplicitSurface2(transform, IsNormalFlipped), _Surfaces(surfaces)
    {}

    ImplicitSurfaceSet2::ImplicitSurfaceSet2(const std::vector<Surface2Ptr>& surfaces,
                    const Transform2& transform, bool IsNormalFlipped)
                : ImplicitSurface2(transform, IsNormalFlipped)
    {
        for (const auto& surface : surfaces)
        {
            AddExplicitSurface(surface);
        }
    }

    ImplicitSurfaceSet2::ImplicitSurfaceSet2(const ImplicitSurfaceSet2& other)
        : ImplicitSurface2(other), _Surfaces(other._Surfaces)
    {}

    size_t ImplicitSurfaceSet2::NumberOfSurfaces() const
    {
        return _Surfaces.size();
    }

    const ImplicitSurface2Ptr& ImplicitSurfaceSet2::SurfaceAt(size_t i) const
    {
        return _Surfaces[i];
    }

    void ImplicitSurfaceSet2::AddExplicitSurface(const Surface2Ptr& surface)
    {
        _Surfaces.push_back(std::make_shared<SurfaceToImplicit2>(surface));
    }

    void ImplicitSurfaceSet2::AddSurface(const ImplicitSurface2Ptr& surface)
    {
        _Surfaces.push_back(surface);
    }

    Vector2D ImplicitSurfaceSet2::ClosestPointLocal(const Vector2D& otherPoint) const
    {
        Vector2D result(kMaxD, kMaxD);

        double minDistance = kMaxD;

        for(const auto& surface : _Surfaces)
        {
            Vector2D localClosestPoint = surface->ClosestPoint(otherPoint);
            double localDistance = surface->ClosestDistance(otherPoint);

            if (localDistance < minDistance)
            {
                result = localClosestPoint;
                minDistance = localDistance;
            }
        }
        return result;
    }

    double ImplicitSurfaceSet2::ClosestDistanceLocal(const Vector2D& otherPoint) const
    {
        double minDistance = kMaxD;

        for (const auto& surface : _Surfaces)
        {
            double localDistance = surface->ClosestDistance(otherPoint);
            minDistance = std::min(localDistance, minDistance);
        }

        return minDistance;
    }

    Vector2D ImplicitSurfaceSet2::ClosestNormalLocal(const Vector2D& otherPoint) const
    {
        Vector2D result(1,0);

        double minDistance = kMaxD;

        for (const auto& surface : _Surfaces)
        {
            Vector2D localClosestNormal = surface->ClosestNormal(otherPoint);
            double localDist = surface->ClosestDistance(otherPoint);

            if (localDist < minDistance)
            {
                result = localClosestNormal;
                minDistance = localDist;
            }
        }

        return result;
    }

    bool ImplicitSurfaceSet2::IntersectsLocal(const Ray2D& ray) const
    {
        for (const auto& surface : _Surfaces)
        {
            if(surface->Intersects(ray))
            {
                return true;
            }
        }
        return false;
    }

    SurfaceRayIntersection2 ImplicitSurfaceSet2::ClosestIntersectionLocal(const Ray2D& ray) const
    {
        SurfaceRayIntersection2 intersection;
        double tMin = kMaxD;

        for (const auto& surface : _Surfaces)
        {
            SurfaceRayIntersection2 localResult;
            localResult = surface->ClosestIntersection(ray);

            if (localResult.IsIntersecting && localResult.t < tMin)
            {
                intersection = localResult;
                tMin = localResult.t;
            }
        }

        return intersection;
    }

    BoundingBox2D ImplicitSurfaceSet2::BoundingBoxLocal() const
    {
        BoundingBox2D bbox;
        for (const auto& surface : _Surfaces)
        {
            bbox.Merge(surface->BoundingBox());
        }

        return bbox;
    }

    double ImplicitSurfaceSet2::SignedDistanceLocal(const Vector2D& otherPoint) const
    {
        double sdf = kMaxD;
        for (const auto& surface : _Surfaces)
        {
            sdf = std::min(sdf, surface->SignedDistance(otherPoint));
        }

        return sdf;
    }

    ImplicitSurfaceSet2::Builder ImplicitSurfaceSet2::builder()
    {
        return Builder();
    }

    ImplicitSurfaceSet2::Builder&
    ImplicitSurfaceSet2::Builder::WithSurfaces(
        const std::vector<ImplicitSurface2Ptr>& surfaces) {
        _Surfaces = surfaces;
        return *this;
    }

    ImplicitSurfaceSet2::Builder&
    ImplicitSurfaceSet2::Builder::WithExplicitSurfaces(
        const std::vector<Surface2Ptr>& surfaces) {
        _Surfaces.clear();
        for (const auto& surface : surfaces) {
            _Surfaces.push_back(std::make_shared<SurfaceToImplicit2>(surface));
        }
        return *this;
    }

    ImplicitSurfaceSet2 ImplicitSurfaceSet2::Builder::Build() const {
        return ImplicitSurfaceSet2(_Surfaces, _transform, _IsNormalFlipped);
    }

    ImplicitSurfaceSet2Ptr ImplicitSurfaceSet2::Builder::MakeShared() const {
        return std::shared_ptr<ImplicitSurfaceSet2>(
            new ImplicitSurfaceSet2(_Surfaces, _transform, _IsNormalFlipped),
            [] (ImplicitSurfaceSet2* obj) {
                delete obj;
            });
    }
}