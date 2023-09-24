#pragma once

#include<Geometry/Surface/surface3.h>

namespace jet
{
    //! \brief 3D Triangle Geometry.
    //!
    //! This class represents 3D triangle geometry which extends Surface3 by
    //! overriding surface-related queries.
    class Triangle3 final : public Surface3
    {
    public:
        class Builder;

        //! Three points of the triangle
        std::array<Vector3D, 3> Points;

        //! Three normals of the surface at three vertices of the triangle.
        std::array<Vector3D,3> Normals;

        //! Three UV coordinates.
        std::array<Vector2D, 3> UVs;

        //! Consructs an empty triangle.
        Triangle3(const Transform3& transform = Transform3(), bool IsNormalFlipped = false);

        //! Constructs a triangle with given \p points, \p normals, \p uvs.
        Triangle3(const std::array<Vector3D,3>& point,
                    const std::array<Vector3D, 3>& normals,
                    const std::array<Vector2D, 3>& uvs,
                    const Transform3& transform = Transform3(),
                    bool IsNormalFlipped = false);
        
        //! Copy Constructor
        Triangle3(const Triangle3& other);

        //! Returns the area of this triangle.
        double Area() const;

        //! Returns the barycentric coordinates for the given point \p pt.
        void GetBarycentricCoords(const Vector3D& pt, double* b0, double* b1, double* b2) const;

        //! Returns the face normal of the triangle.
        Vector3D FaceNormal() const;

        //! Set Triangle3::Normals to the face normal.
        void SetNormalsToFaceNormal();

        //! Returns builder for Triangle3.
        static Builder builder();
    protected:
        Vector3D ClosestPointLocal(const Vector3D& otherPoint) const override;

        bool IntersectsLocal(const Ray3D& ray) const override;

        BoundingBox3D BoundingBoxLocal() const override;

        Vector3D ClosestNormalLocal(
            const Vector3D& otherPoint) const override;

        SurfaceRayIntersection3 ClosestIntersectionLocal(
            const Ray3D& ray) const override;
    };

    typedef std::shared_ptr<Triangle3> Triangle3Ptr;

    //! \brief Front-end to create Triangle3 objects step by step.
    //!
    class Triangle3::Builder final
        : public SurfaceBuilderBase3<Triangle3::Builder> {
    public:
        //! Returns builder with points.
        Builder& WithPoints(const std::array<Vector3D, 3>& points);

        //! Returns builder with normals.
        Builder& WithNormals(const std::array<Vector3D, 3>& normals);

        //! Returns builder with uvs.
        Builder& WithUVs(const std::array<Vector2D, 3>& uvs);

        //! Builds Triangle3.
        Triangle3 Build() const;

        //! Builds shared pointer of Triangle3 instance.
        Triangle3Ptr MakeShared() const;

    private:
        std::array<Vector3D, 3> _points;
        std::array<Vector3D, 3> _normals;
        std::array<Vector2D, 3> _uvs;
    };

}