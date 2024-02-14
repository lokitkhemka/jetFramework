#include <jet.h>
#include "collider2_set.h"
#include <vector>

namespace jet
{
    ColliderSet2::ColliderSet2() : ColliderSet2(std::vector<Collider2Ptr>())
    {}

    ColliderSet2::ColliderSet2(const std::vector<Collider2Ptr>& others)
    {
        SetSurface(std::make_shared<SurfaceSet2>());
        for (const auto& collider: others)
        {
            AddCollider(collider);
        }
    }

    Vector2D ColliderSet2::VelocityAt(const Vector2D& point) const
    {
        size_t ClosestCollider = kMaxSize;
        double ClosestDist = kMaxD;

        for (size_t i = 0; i < _Colliders.size(); ++i)
        {
            double dist = _Colliders[i]->Surface()->ClosestDistance(point);
            if (dist < ClosestDist)
            {
                ClosestDist = dist;
                ClosestCollider = i;
            }
        }

        if (ClosestCollider != kMaxSize)
        {
            return _Colliders[ClosestCollider]->VelocityAt(point);
        }
        else
        {
            return Vector2D();
        }
    }

    void ColliderSet2::AddCollider(const Collider2Ptr& collider)
    {
        auto SurfaceSet = std::dynamic_pointer_cast<SurfaceSet2>(Surface());
        _Colliders.push_back(collider);
        SurfaceSet->AddSurface(collider->Surface());
    }

    size_t ColliderSet2::NumberOfColliders() const
    {
        return _Colliders.size();
    }

    Collider2Ptr ColliderSet2::Collider(size_t i) const
    {
        return _Colliders[i];
    }

    ColliderSet2::Builder ColliderSet2::builder()
    {
        return Builder();
    }

    ColliderSet2::Builder& ColliderSet2::Builder::WithColliders(const std::vector<Collider2Ptr>& others)
    {
        _Colliders = others;
        return *this;
    }


    ColliderSet2 ColliderSet2::Builder::Build() const
    {
        return ColliderSet2(_Colliders);
    }

    ColliderSet2Ptr ColliderSet2::Builder::MakeShared() const
    {
        return std::shared_ptr<ColliderSet2>(new ColliderSet2(_Colliders), [](ColliderSet2* obj)
                {
                    delete obj;
                });
    }

}