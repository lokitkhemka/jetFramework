#include <jet.h>
#include "surface2_set.h"

#include <algorithm>
#include <limits>
#include <vector>

namespace jet
{
    SurfaceSet2::SurfaceSet2()
    {}

    SurfaceSet2::SurfaceSet2(const std::vector<Surface2Ptr>& others,
                const Transform2& transform, bool IsNormalFlipped)
                : Surface2(transform, IsNormalFlipped), _Surfaces(others)
    {}

    SurfaceSet2::SurfaceSet2(const SurfaceSet2& other)
            : Surface2(other), _Surfaces(other._Surfaces)
    {}

    size_t SurfaceSet2::NumberOfSurfaces() const
    {
        return _Surfaces.size();
    }

    const Surface2Ptr& SurfaceSet2::SurfaceAt(size_t i) const
    {
        return _Surfaces[i];
    }

    void SurfaceSet2::AddSurface(const Surface2Ptr& surface)
    {
        _Surfaces.push_back(surface);
    }

    Vector2D SurfaceSet2::ClosestPointLocal(const Vector2D& otherPoint) const
    {
        Vector2D result(std::numeric_limits<double>::max(), std::numeric_limits<double>::max());

        double MinDistance = std::numeric_limits<double>::max();

        for (const auto& surface:_Surfaces)
        {
            Vector2D LocalClosestPoint = surface->ClosestPoint(otherPoint);
            double LocalDistance = surface->ClosestDistance(otherPoint);

            if (LocalDistance < MinDistance)
            {
                result = LocalClosestPoint;
                MinDistance = LocalDistance;
            }
        }

        return result;
    }

    Vector2D SurfaceSet2::ClosestNormalLocal(const Vector2D& otherPoint) const
    {
        Vector2D result(1,0);
        double MinDistance = std::numeric_limits<double>::max();

        for (const auto& surface : _Surfaces)
        {
            Vector2D LocalClosestNormal = surface->ClosestNormal(otherPoint);
            double LocalDistance = surface->ClosestDistance(otherPoint);

            if (LocalDistance < MinDistance)
            {
                result = LocalClosestNormal;
                MinDistance = LocalDistance;
            }
        }
        return result;
    }

    double SurfaceSet2::ClosestDistanceLocal(const Vector2D& otherPoint) const
    {
        double MinDistance = std::numeric_limits<double>::max();
        for (const auto& surface : _Surfaces)
        {
            double LocalDistance = surface->ClosestDistance(otherPoint);
            MinDistance = std::min(MinDistance, LocalDistance);
        }

        return MinDistance;
    }

    bool SurfaceSet2::IntersectsLocal(const Ray2D& ray) const
    {
        for (const auto& surface : _Surfaces)
        {
            if (surface->Intersects(ray))
                return true;
        }
        return false;
    }

    SurfaceRayIntersection2 SurfaceSet2::ClosestIntersectionLocal(const Ray2D& ray) const
    {
        SurfaceRayIntersection2 Intersection;
        double tMin = kMaxD;

        for (const auto& surface : _Surfaces)
        {
            SurfaceRayIntersection2 LocalResult;
            LocalResult = surface->ClosestIntersection(ray);

            if (LocalResult.IsIntersecting && LocalResult.t < tMin)
            {
                Intersection = LocalResult;
                tMin = LocalResult.t;
            }
        }
        return Intersection;
    }

    BoundingBox2D SurfaceSet2::BoundingBoxLocal() const
    {
        BoundingBox2D bbox;
        for (const auto& surface : _Surfaces)
        {
            bbox.Merge(surface->BoundingBox());
        }
        return bbox;
    }

    SurfaceSet2::Builder SurfaceSet2::builder()
    {
        return Builder();
    }

    SurfaceSet2::Builder& SurfaceSet2::Builder::WithSurfaces(const std::vector<Surface2Ptr>& others)
    {
        _Surfaces = others;
        return *this;
    }

    SurfaceSet2 SurfaceSet2::Builder::Build() const
    {
        return SurfaceSet2(_Surfaces, _transform, _IsNormalFlipped);
    }

    SurfaceSet2Ptr SurfaceSet2::Builder::MakeShared() const
    {
        return std::shared_ptr<SurfaceSet2>(new SurfaceSet2(_Surfaces, _transform, _IsNormalFlipped),
                [](SurfaceSet2* obj){
                    delete obj;
                });
    }
}