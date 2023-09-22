#pragma once

#include <Geometry/Surface/surface3.h>

namespace jet
{
    //! \brief 3D plane geometry.
    //! This class represents 3D plane geometry which extends Surface3 by
    //! overriding surface-related queries.
    class Plane3 final : public Surface3
    {
    public:
        class Builder;

        //! Plane Normal.
        Vector3D Normal = Vector3D(0,1,0);

        //! Point that lies on the plane.
        Vector3D Point;

        //! Constructs a plane that crosses (0, 0, 0) with surface normal (0, 1, 0).
        Plane3(const Transform3& transform = Transform3(), bool IsNormalFlipped = false);

        //! Constructs a plane that cross \p point with surface normal \p normal.
        Plane3(const Vector3D& normal, const Vector3D& point,
                const Transform3& transform = Transform3(), bool IsNormalFlipped = false);
        
        //! Consturcts a plane with three points on the surface. The normal will be set
        //! using the counter clockwise direction.
        Plane3(const Vector3D& point0, const Vector3D& point1,
                const Vector3D& point2, const Transform3& transform = Transform3(),
                    bool IsNormalFlipped = false);

        //! Copy constructor.
        Plane3(const Plane3& other);

        //! Returns builder for Plane3.
        static Builder builder();

    protected:
        Vector3D ClosestPointLocal(const Vector3D& otherPoint) const override;

        bool IntersectsLocal(const Ray3D& ray) const override;

        BoundingBox3D BoundingBoxLocal() const override;

        Vector3D ClosestNormalLocal(const Vector3D& otherPoint) const override;

        SurfaceRayIntersection3 ClosestIntersectionLocal(const Ray3D& ray) const override;
    };

    typedef std::shared_ptr<Plane3> Plane3Ptr;


    //! \brief Frontend to create Plane3 objects step by step.

    class Plane3::Builder final : public SurfaceBuilderBase3<Plane3::Builder>
    {
    public:
        //! Returns builder with plane normal.
        Builder& WithNormal(const Vector3D& normal);

        //! Returns builder with point on the plane.
        Builder& WithPoint(const Vector3D& point);

        //! Builds Plane3
        Plane3 Build() const;

        //! Builds Shared pointer of Plane3 instance.
        Plane3Ptr MakeShared() const;

    private:
        Vector3D _normal{0,1,0};
        Vector3D _point{0,0,0};
    };
}