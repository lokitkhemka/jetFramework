#include <jet.h>
#include "rigid_body3_collider.h"

namespace jet
{
    RigidBodyCollider3::RigidBodyCollider3(const Surface3Ptr& surface)
    {
        SetSurface(surface);
    }

    RigidBodyCollider3::RigidBodyCollider3(const Surface3Ptr& surface, const Vector3D& linearVelocity,
                                const Vector3D& angularVelocity)
                        : LinearVelocity(linearVelocity), AngularVelocity(angularVelocity)
    {
        SetSurface(surface);
    }

    Vector3D RigidBodyCollider3::VelocityAt(const Vector3D& point) const
    {
        Vector3D r = point - Surface()->transform.Translation();
        return LinearVelocity + AngularVelocity.Cross(r);
    }

    RigidBodyCollider3::Builder RigidBodyCollider3::builder()
    {
        return Builder();
    }

    RigidBodyCollider3::Builder& RigidBodyCollider3::Builder::WithSurface(const Surface3Ptr& surface)
    {
        _Surface = surface;
        return *this;
    }

    RigidBodyCollider3::Builder& RigidBodyCollider3::Builder::WithLinearVelocity(const Vector3D& linearVelocity)
    {
        _LinearVelocity = linearVelocity;
        return *this;
    }

    RigidBodyCollider3::Builder& RigidBodyCollider3::Builder::WithAngularVelocity(const Vector3D& angularVelocity)
    {
        _AngularVelocity = angularVelocity;
        return *this;
    }

    RigidBodyCollider3 RigidBodyCollider3::Builder::Build() const
    {
        return RigidBodyCollider3(_Surface, _LinearVelocity, _AngularVelocity);
    }

    RigidBodyCollider3Ptr RigidBodyCollider3::Builder::MakeShared() const
    {
        return std::shared_ptr<RigidBodyCollider3>(
            new RigidBodyCollider3(_Surface, _LinearVelocity, _AngularVelocity),
            [](RigidBodyCollider3* obj)
            {
                delete obj;
        });
    }
}