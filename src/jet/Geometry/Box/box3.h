#pragma once

#include<Geometry/Surface/surface3.h>
#include<Geometry/BoundingBox/bounding_box3.h>

namespace jet
{
    //! \brief 3D Axis Aligned Box Geometry
    //! This class represents 3D box geometry which extends Surface3 by overriding
    //! surface-related queries. This wraps BoundingBox3D.
    class Box3 final : public Surface3
    {
    public:
        class Builder;

        //! Bounding Box of this box.
        BoundingBox3D Bound = BoundingBox3D(Vector3D(), Vector3D(1.,1.,1.));

        //! Constructs (0,0,0) x (1,1,1) box.
        Box3(const Transform3& transform = Transform3(), bool IsNormalFlipped = false);

        //! Constructs a box with given \p lowerCorner and \p upperCorner.
        Box3(const Vector3D& lowerCorner, const Vector3D& upperCorner, const Transform3& transform = Transform3(),
                    bool IsNormalFlipped = false);
        
        //! Constructs a box with BoundingBox3D instance.
        explicit Box3(const BoundingBox3D& boundingBox, const Transform3& transform = Transform3(),
                        bool IsNormalFlipped = false);

        //! Copy Constructor
        Box3(const Box3& other);

        //! Returns builder for Box3.
        static Builder builder();

    protected:
        Vector3D ClosestPointLocal(const Vector3D& otherPoint) const override;

        bool IntersectsLocal(const Ray3D& ray) const override;

        BoundingBox3D BoundingBoxLocal() const override;

        Vector3D ClosestNormalLocal(const Vector3D& otherPoint) const override;

        SurfaceRayIntersection3 ClosestIntersectionLocal(
            const Ray3D& ray) const override;
    };

    typedef std::shared_ptr<Box3> Box3Ptr;

    //! \brief Front-end to create Box3 objects step by step.
    //!
    class Box3::Builder final : public SurfaceBuilderBase3<Box3::Builder> {
    public:
        //! Returns builder with lower corner set.
        Builder& WithLowerCorner(const Vector3D& pt);

        //! Returns builder with upper corner set.
        Builder& WithUpperCorner(const Vector3D& pt);

        //! Returns builder with bounding box.
        Builder& WithBoundingBox(const BoundingBox3D& bbox);

        //! Builds Box3.
        Box3 Build() const;

        //! Builds shared pointer of Box3 instance.
        Box3Ptr MakeShared() const;

    private:
        Vector3D _lowerCorner{0, 0, 0};
        Vector3D _upperCorner{1, 1, 1};
    };
}