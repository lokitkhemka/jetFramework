#include <jet.h>
#include "rigid_body2_collider.h"

namespace jet
{
    RigidBodyCollider2::RigidBodyCollider2(const Surface2Ptr& surface)
    {
        SetSurface(surface);
    }

    RigidBodyCollider2::RigidBodyCollider2(const Surface2Ptr& surface, const Vector2D& linearVelocity,
                        double angularVelocity) : LinearVelocity(linearVelocity),
                        AngularVelocity(angularVelocity)
    {
        SetSurface(surface);
    }

    Vector2D RigidBodyCollider2::VelocityAt(const Vector2D& point) const
    {
        Vector2D r = point - Surface()->transform.Translation();
        return LinearVelocity + AngularVelocity * Vector2D(-r.y, r.x);
    }

    RigidBodyCollider2::Builder RigidBodyCollider2::builder()
    {
        return Builder();
    }

    RigidBodyCollider2::Builder& RigidBodyCollider2::Builder::WithSurface(const Surface2Ptr& surface)
    {
        _Surface = surface;
        return *this;
    }

    RigidBodyCollider2::Builder& RigidBodyCollider2::Builder::WithLinearVelocity(const Vector2D& linearVelocity)
    {
        _LinearVelocity = linearVelocity;
        return *this;
    }

    RigidBodyCollider2::Builder& RigidBodyCollider2::Builder::WithAngularVelocity(double angularVelocity)
    {
        _AngularVelocity = angularVelocity;
        return *this;
    }

    RigidBodyCollider2 RigidBodyCollider2::Builder::Build() const
    {
        return RigidBodyCollider2(_Surface, _LinearVelocity, _AngularVelocity);
    }

    RigidBodyCollider2Ptr RigidBodyCollider2::Builder::MakeShared() const
    {
        return std::shared_ptr<RigidBodyCollider2>(new RigidBodyCollider2(_Surface, _LinearVelocity, _AngularVelocity),
                    [](RigidBodyCollider2* obj){
                        delete obj;
                    });
    }
}