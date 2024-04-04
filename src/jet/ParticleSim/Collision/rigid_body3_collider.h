#pragma once

#include <ParticleSim/Collision/collider3.h>
#include <Geometry/quaternion.h>

namespace jet
{
    //! \brief 3D rigid body collider class.
    class RigidBodyCollider3 final : public Collider3
    {
    public:
        class Builder;

        //! Linear velocity of the rigid body.
        Vector3D LinearVelocity;

        //! Angular velocity of the rigid body.
        Vector3D AngularVelocity;

        //! Constructs a collider with a surface.
        explicit RigidBodyCollider3(const Surface3Ptr& surface);

        //! Constructs a collider with parameters
        RigidBodyCollider3(const Surface3Ptr& surface, const Vector3D& linearVelocity,
                        const Vector3D& angularVelocity);
        
        //! Returns the velocity at the given \p point.
        Vector3D VelocityAt(const Vector3D& point) const override;

        //! Returns builder for RigidBodyCollider3
        static Builder builder();
    };

    typedef std::shared_ptr<RigidBodyCollider3> RigidBodyCollider3Ptr;

    //! \brief Frontend to create RigidBodyCollider3 objects
    class RigidBodyCollider3::Builder final
    {
    public:
        //! Returns builder with surface.
        Builder& WithSurface(const Surface3Ptr& surface);

        //! Returns builder with linear velocity
        Builder& WithLinearVelocity(const Vector3D& linearVelocity);

        //! Returns builder with angular velocity.
        Builder& WithAngularVelocity(const Vector3D& angularVelocity);

        //! Builds RigidBodyCollider3
        RigidBodyCollider3 Build() const;

        //! Builds shared pointer of RigidBodyCollider3 instance.
        RigidBodyCollider3Ptr MakeShared() const;

    private:
        Surface3Ptr _Surface;
        Vector3D _LinearVelocity{0, 0, 0};
        Vector3D _AngularVelocity{0, 0, 0};
    };
}