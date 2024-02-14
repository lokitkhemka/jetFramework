#pragma once

#include <ParticleSim/Collision/collider2.h>
#include <Geometry/Surface/surface2_set.h>
#include <vector>

namespace jet
{
    //! Collection of 2D colliders
    class ColliderSet2 final : public Collider2
    {
    public:
        class Builder;

        //! Default Constructor
        ColliderSet2();

        //! Constructs with other colliders
        explicit ColliderSet2(const std::vector<Collider2Ptr>& others);

        //! Returns the velocity of the collider at given \p Point.
        Vector2D VelocityAt(const Vector2D& point) const override;

        //! Adds a collider to the set.
        void AddCollider(const Collider2Ptr& collider);

        //! Returns the number of colliders
        size_t NumberOfColliders() const;

        //! Returns collider at index \p i.
        Collider2Ptr Collider(size_t i) const;

        //! Returns Builder for ColliderSet2
        static Builder builder();
    private:
        std::vector<Collider2Ptr> _Colliders;
    };

    typedef std::shared_ptr<ColliderSet2> ColliderSet2Ptr;

    //! \brief Front end to create ColliderSet2 object instance.
    class ColliderSet2::Builder final
    {
    public:
        //! Returns Builder with other colliders.
        Builder& WithColliders(const std::vector<Collider2Ptr>& others);
        
        //! Builds ColliderSet2
        ColliderSet2 Build() const;

        //! Builds Shared pointer of ColliderSet2 instance.
        ColliderSet2Ptr MakeShared() const;
    private:
        std::vector<Collider2Ptr> _Colliders;
    };
}