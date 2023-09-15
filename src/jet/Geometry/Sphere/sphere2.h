#pragma once

#include<Geometry/Surface/surface2.h>
#include<Geometry/BoundingBox/bounding_box2.h>

namespace jet
{
    //! \brief 2D sphere (circle) geometry
    //! This class represents a circle which extends Surface2 by
    //! overriding surface-related queries
    //!

    class Sphere2 final : public Surface2
    {
    public:
        class Builder;

        //! Center of the sphere
        Vector2D Center;

        //! Radius of the sphere
        double Radius = 1.0;

        //! Constructs a sphere with center at (0,0) and radius of 1.
        Sphere2(const Transform2& transform = Transform2(), bool IsNormalFlipped = false);

        //! Constructs a sphere with \p center and \p radius.
        Sphere2(const Vector2D& center, double radius, 
                const Transform2& transform = Transform2(), bool IsNormalFlipped = false);

        //! Copy constructor
        Sphere2(const Sphere2& other);

        //! Returns Builder for Sphere2
        static Builder builder();
    
    private:
        Vector2D ClosestPointLocal(const Vector2D& otherPoint) const override;

        double ClosestDistanceLocal(const Vector2D& otherPoint) const override;

        bool IntersectsLocal(const Ray2D& ray) const override;

        BoundingBox2D BoundingBoxLocal() const override;

        Vector2D ClosestNormalLocal(const Vector2D& otherPoint) const override;

        SurfaceRayIntersection2 ClosestIntersectionLocal(const Ray2D& ray) const override;
    };

    typedef std::shared_ptr<Sphere2> Sphere2Ptr;

    //!
    //! \brief Frontend to create Sphere2 objects.
    class Sphere2::Builder final : public SurfaceBuilderBase2<Sphere2::Builder>
    {
    public:
        //! Returns Builder with sphere center.
        Builder& WithCenter(const Vector2D& center);

        //! Returns Builder with sphere radius.
        Builder& WithRadius(double radius);

        //! Builds Sphere2
        Sphere2 Build() const;

        //! Builds Shared Pointer of Sphere2 instance
        Sphere2Ptr MakeShared() const;

    private:
        Vector2D _center{0,0};
        double _radius = 0.0;
    };
}
