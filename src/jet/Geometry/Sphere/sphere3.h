#pragma once

#include<Geometry/Surface/surface3.h>
#include<Geometry/BoundingBox/bounding_box3.h>

namespace jet
{
    //! \brief 3D sphere geometry
    //! This class represents 3D sphere geometry which extends Surface3 by
    //! overriding surface-related queries.

    class Sphere3 final : public Surface3
    {
    public:
        class Builder;

        //! Center of the sphere
        Vector3D Center;

        //! Radius of the sphere
        double Radius = 1.0;

        //! Constructs a sphere with center at (0, 0, 0) and radius of 1.
        Sphere3(const Transform3& transform = Transform3(), bool IsNormalFlipped = false);

        //! Constructs a sphere with \p Center and \p Radius.
        Sphere3(const Vector3D& center, double radius, const Transform3& transform = Transform3(),
                    bool isNormalFlipped = false);
        
        //! Copy constructor
        Sphere3(const Sphere3& other);

        //! Returns builder for Sphere3.
        static Builder builder();

    private:
        Vector3D ClosestPointLocal(const Vector3D& otherPoint) const override;

        double ClosestDistanceLocal(const Vector3D& otherPoint) const override;

        bool IntersectsLocal(const Ray3D& ray) const override;

        BoundingBox3D BoundingBoxLocal() const override;

        Vector3D ClosestNormalLocal(const Vector3D& otherPoint) const override;

        SurfaceRayIntersection3 ClosestIntersectionLocal(const Ray3D& ray) const override;
    };

    typedef std::shared_ptr<Sphere3> Sphere3Ptr;

    //!
    //! \brief Front-end to create Sphere3 objects step by step.
    //!
    class Sphere3::Builder final : public SurfaceBuilderBase3<Sphere3::Builder>{
    public:
        //! Returns builder with sphere center.
        Builder& WithCenter(const Vector3D& center);

        //! Returns builder with sphere radius.
        Builder& WithRadius(double radius);

        //! Builds Sphere3.
        Sphere3 Build() const;

        //! Builds shared pointer of Sphere3 instance.
        Sphere3Ptr MakeShared() const;

    private:
        Vector3D _center{0, 0, 0};
        double _radius = 0.0;
    };
}