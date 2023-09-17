#pragma once

#include<Geometry/Surface/surface2.h>
#include<Geometry/BoundingBox/bounding_box2.h>

namespace jet
{
    //! \brief 2D Box Geometry.
    //!
    //! This class represents 2D box geomery which extends Surface2 by overriding
    //! surface-related queries. This class implements axis-aligned box that wraps
    //! BoundingBox2D.

    class Box2 final: public Surface2
    {
    public:
        class Builder;

        //! Bounding box of this box.
        BoundingBox2D Bound = BoundingBox2D(Vector2D(), Vector2D(1.0,1.0));

        //! Constructs a (0,0) x (1,1) box.
        Box2(const Transform2& transform = Transform2(), bool isNormalFlipped = false);

        //! Constructs a box with given \p lowerCorner and \p upperCorner.
        Box2(const Vector2D& lowerCorner, const Vector2D& upperCorner, 
            const Transform2& transform = Transform2(), bool isNormalFlipped = false);
        
        //! Constructs a box with BoundingBox2D instance.
        Box2(const BoundingBox2D& boundingBox, const Transform2& transform = Transform2(),
                bool isNormalFlipped = false);
        
        
        //! Copy constructor
        Box2(const Box2& other);

        //! Returns Builder for Box2.
        static Builder builder();

    protected:
        Vector2D ClosestPointLocal(const Vector2D& otherPoint) const override;

        bool IntersectsLocal(const Ray2D& ray) const override;

        BoundingBox2D BoundingBoxLocal() const override;

        Vector2D ClosestNormalLocal(const Vector2D& otherPoint) const override;

        SurfaceRayIntersection2 ClosestIntersectionLocal(const Ray2D& ray) const override;
    };

    typedef std::shared_ptr<Box2> Box2Ptr;

    //! \brief Frontend to Create Box2 Objects.
    class Box2::Builder final : public SurfaceBuilderBase2<Box2::Builder>
    {
    public:
        //! Returns builder with lower corner set.
        Builder& WithLowerCorner(const Vector2D& pt);

        //! Returns builder with upper corner set.
        Builder& WithUpperCorner(const Vector2D& pt);

        //! Returns Builder with bounding Box
        Builder& WithBoundingBox(const BoundingBox2D& bbox);

        //! Builds Box2
        Box2 Build() const;

        //! Builds Shared pointer of Box2 instance.
        Box2Ptr MakeShared() const;

    private:
        Vector2D _lowerCorner{0,0};
        Vector2D _upperCorner{1,1};
    };

    
}