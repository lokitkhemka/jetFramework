#pragma once

#include<Geometry/Surface/surface2.h>

namespace jet
{
    //! \brief 2D Plane geometry
    //!
    //! This class represents 2D plane geometry which extends Surface2
    //! by overriding surface-related queries.
    class Plane2 final: public Surface2
    {
    public:
        class Builder;
        
        //! Plane Normal
        Vector2D Normal = Vector2D(0,1);

        //! Point that lies on the plane
        Vector2D Point;

        //! Constructs a plane that crosses (0,0) with surface normal (0,1)
        Plane2(const Transform2& transform = Transform2(), bool IsNormalFlipped = false);

        //! Constructs a plane that cross \p point and with surface normal \p normal.
        Plane2(const Vector2D& normal, const Vector2D& point,
                const Transform2& transform = Transform2(), bool IsNormalFlipped = false);
        
        //! Copy constructor
        Plane2(const Plane2& other);

        //! Returns the builder for Plane2
        static Builder builder();
    
    private:

        Vector2D ClosestPointLocal(const Vector2D& otherPoint) const override;

        bool IntersectsLocal(const Ray2D& ray) const override;

        BoundingBox2D BoundingBoxLocal() const override;

        Vector2D ClosestNormalLocal(const Vector2D& otherPoint) const override;

        SurfaceRayIntersection2 ClosestIntersectionLocal(const Ray2D& ray) const override;
    };

    typedef std::shared_ptr<Plane2> Plane2Ptr;


    //! \brief Frontend to create Plane2 object.

    class Plane2::Builder final : public SurfaceBuilderBase2<Plane2::Builder>
    {
    public:
        //! Returns the builder with plane normal.
        Builder& WithNormal(const Vector2D& normal);

        //! Returns the builder with point on the plane.
        Builder& WithPoint(const Vector2D& point);

        //! Builds Plane2.
        Plane2 Build() const;

        //! Builds Shared pointer of Plane2 instance.
        Plane2Ptr MakeShared() const;
    private:
        Vector2D _normal{0,1};
        Vector2D _point{0,0};
    };
}