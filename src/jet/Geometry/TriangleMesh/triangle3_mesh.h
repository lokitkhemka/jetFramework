#pragma once

#include <Arrays/array1.h>
#include <Points/point3.h>
#include <Geometry/quaternion.h>
#include <Geometry/Surface/surface3.h>
#include <Geometry/TriangleMesh/triangle3.h>
#include <iostream>
#include <utility>

namespace jet
{
    //! \brief 3D Triangle Mesh Geometry class
    //! This class represents 3D triangle mesh goemetry which extends Surface3
    //! by overriding surface-related queries. The mesh structure stores points,
    //! normals and UV coordinates.

    class TriangleMesh3 final : public Surface3
    {
    public:
        class Builder;

        typedef Array1<Vector2D> Vector2DArray;
        typedef Array1<Vector3D> Vector3DArray;
        typedef Array1<Point3UI> IndexArray;

        typedef Vector3DArray PointArray;
        typedef Vector3DArray NormalArray;
        typedef Vector2DArray UVArray;

        //! Default Constructor
        TriangleMesh3(const Transform3& transform = Transform3(),
                        bool IsNormalFlipped = false);
        
        //! Constructs mesh with points, normals, uvs and their indices.
        TriangleMesh3(const PointArray& Points, const NormalArray& Normals,
                        const UVArray& UVs, const IndexArray& PointIndices,
                        const IndexArray& NormalIndices, const IndexArray& UVIndices, 
                        const Transform3& transform = Transform3(),
                        bool IsNormalFlipped = false);

        //! Copy constructor
        TriangleMesh3(const TriangleMesh3& other);

        //! Clears all content.
        void Clear();

        //! Copies the contest from other mesh.
        void Set(const TriangleMesh3& other);

        //! Swaps the contents with \p other mesh.
        void Swap(TriangleMesh3& other);

        //! Returns Area of this mesh.
        double Area() const;

        //! Returns the volume of teh mesh.
        double Volume() const;

        //! Returns const reference to the \p i-th Point.
        const Vector3D& Point(size_t i) const;

        //! Return reference to the \p i-th point.
        Vector3D& Point(size_t i);

        //! Returns const reference to the \p i-th normal.
        const Vector3D& Normal(size_t i) const;

        //! Returns reference to the \p i-th normal.
        Vector3D& Normal(size_t i);

        //! Returns const reference to the \p i th UV coordinates.
        const Vector2D& UV(size_t i) const;

        //! Returns the reference to the \p i th UV coordinates.
        Vector2D& UV(size_t i);

        //! Returns const reference to the point indices of \p i th triangle.
        const Point3UI& PointIndex(size_t i) const;

        //! Returns reference to the point indices of \p i th triangle.
        Point3UI& PointIndex(size_t i);

        //! Returns const reference to the normal indices of \p i th triangle.
        const Point3UI& NormalIndex(size_t i) const;

        //! Returns reference to the normal indices of \p i th triangle.
        Point3UI& NormalIndex(size_t i);

        //! Returns const reference to the UV indices of \p i th triangle.
        const Point3UI& UVIndex(size_t i) const;

        //! Returns reference to the UV indices of \p i th triangle.
        Point3UI& UVIndex(size_t i);

        //! Returns \p i th triangle.
        Triangle3 Triangle(size_t i) const;

        //! Returns number of points.
        size_t NumberOfPoints() const;

        //! Returns number of normals.
        size_t NumberOfNormals() const;

        //! Returns number of UV Coordinates.
        size_t NumberOfUVs() const;

        //! Returns number of triangles.
        size_t NumberOfTriangles() const;

        //! Returns true if the mesh has normals.
        bool HasNormals() const;

        //! Returns true if the mesh has UV coordinates.
        bool HasUVs() const;

        //! Adds a Point
        void AddPoint(const Vector3D& pt);

        //! Adds a Normal.
        void AddNormal(const Vector3D& n);

        //! Adds a UV.
        void AddUV(const Vector2D& t);

        //! Adds a triangle with points.
        void AddPointTriangle(const Point3UI& NewPointIndices);

        //! Adds a triangle with point and normal.
        void AddPointNormalTriangle(const Point3UI& NewPointIndices, const Point3UI& NewNormalIndices);

        //! Adds a triangle with point, normal and UV.
        void AddPointUVNormalTriangle(const Point3UI& NewPointIndices,
                                        const Point3UI& NewUVIndices, const Point3UI& NewNormalIndices);


        //! Adds a triangle with Point and UV.
        void AddPointUVTriangle(const Point3UI& NewPointIndices, const Point3UI& NewUVIndices);

        //! Add a triangle
        void AddTriangle(const Triangle3& tri);

        //! Sets entire normals to the face normals.
        void SetFaceNormal();

        //! Sets angle weighted vertex normal.
        void SetAngleWeightedVertexNormal();

        //! Scales the mesh by given factor.
        void Scale(double factor);

        //! Translates the mesh.
        void Translate(const Vector3D& t);

        //! Rotates the mesh.
        void Rotate(const QuaternionD& q);

        //! Writes the mesh in obj format to the output stream.
        void WriteObj(std::ostream* strm) const;

        //! Reads the mesh in obj format to the output stream.
        bool ReadObj(std::istream* strm);

        //! Copies \p other Triangle mesh.
        TriangleMesh3& operator=(const TriangleMesh3& other);

        //! Returns builder for TriangleMesh3.
        static Builder builder();
    
    protected:
        Vector3D ClosestPointLocal(const Vector3D& otherPoint) const override;

        double ClosestDistanceLocal(const Vector3D& otherPoint) const override;

        bool IntersectsLocal(const Ray3D& ray) const override;

        BoundingBox3D BoundingBoxLocal() const override;

        Vector3D ClosestNormalLocal(const Vector3D& otherPoint) const override;

        SurfaceRayIntersection3 ClosestIntersectionLocal(const Ray3D& ray) const override;
    
    private:
        PointArray _Points;
        NormalArray _Normals;
        UVArray _UVs;
        IndexArray _PointIndices;
        IndexArray _NormalIndices;
        IndexArray _UVIndices;
    };

    typedef std::shared_ptr<TriangleMesh3> TriangleMesh3Ptr;


    //! \brief Frontend to create TriangleMesh3 objects step by step.
    class TriangleMesh3::Builder final : public SurfaceBuilderBase3<TriangleMesh3::Builder>
    {
    public:
        //! Returns builder with points.
        Builder& WithPoints(const PointArray& Points);

        //! Returns builder with normals.
        Builder& WithNormals(const NormalArray& Normals);

        //! Returns builder with UVs.
        Builder& WithUVs(const UVArray& UVs);

        //! Returns builder with point indices.
        Builder& WithPointIndices(const IndexArray& PointIndices);

        //! Returns builder with normal indices.
        Builder& WithNormalIndices(const IndexArray& NormalIndices);

        //! Returns builder with UV indices.
        Builder& WithUVIndices(const IndexArray& UVIndices);

        //! Builds TriangleMesh3
        TriangleMesh3 Build() const;

        //! Builds shared point of TriangleMesh3 instance.
        TriangleMesh3Ptr MakeShared() const;

    private:
        PointArray _Points;
        NormalArray _Normals;
        UVArray _UVs;
        IndexArray _PointIndices;
        IndexArray _NormalIndices;
        IndexArray _UVIndices;

    };
}