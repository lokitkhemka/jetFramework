#pragma once

#include <ParticleSim/Collision/collider2.h>

namespace jet
{
    //! \brief 2D Rigid Body Collider class.
    //!
    //! This class implements 2D rigid body collider. The collider only takes
    //! rigid body motion with linear and rotational velocities.

    class RigidBodyCollider2 final : public Collider2
    {
    public:
        class Builder;

        //! Linear Velocity of the Rigid Body.
        Vector2D LinearVelocity;

        //! Angular Velocity of the Rigid Body.
        double AngularVelocity = 0.0;

        //! Constructs a Collider with a surface.
        explicit RigidBodyCollider2(const Surface2Ptr& surface);

        //! Constructs a collider with a surface and other parameters.
        RigidBodyCollider2(const Surface2Ptr& surface, const Vector2D& LinearVelocity,
                            double AngularVelocity);
        
        //! Returns the velocity of the collider at given \p point.
        Vector2D VelocityAt(const Vector2D& point) const override;

        //! Returns builder for RigidBodyCollider2
        static Builder builder();
    };

    //! Shared pointer for RigidBodyCollider2 type.
    typedef std::shared_ptr<RigidBodyCollider2> RigidBodyCollider2Ptr;

    //! \brief Frontend to create RigidBodyCollider2 objects.
    class RigidBodyCollider2::Builder final
    {
    public:
        //! Returns builder with surface.
        Builder& WithSurface(const Surface2Ptr& surface);

        //! Returns the builder with Linear Velocity.
        Builder& WithLinearVelocity(const Vector2D& linearVelocity);

        //! Returns the builder with Angular Velocity.
        Builder& WithAngularVelocity(double angularVelocity);

        //! Builds RigidBodyCollider2
        RigidBodyCollider2 Build() const;

        //! Buils shared pointer of RigidBodyCollider2 instance.
        RigidBodyCollider2Ptr MakeShared() const;
    private:
        Surface2Ptr _Surface;
        Vector2D _LinearVelocity{0,0};
        double _AngularVelocity = 0.0;
    };
}