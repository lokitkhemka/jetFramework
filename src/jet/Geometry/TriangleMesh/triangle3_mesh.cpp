#include<jet.h>
#include<parallel.h>
#include<obj/obj_parser.hpp>
#include"triangle3_mesh.h"
#include<algorithm>
#include<limits>
#include<string>
#include<utility>

namespace jet
{
    inline std::ostream& operator<<(std::ostream& strm, const Vector2D& v)
    {
        strm << v.x << ' ' << v.y;
        return strm;
    }

    inline std::ostream& operator<<( std::ostream& strm, const Vector3D& v)
    {
        strm << v.x << ' ' << v.y << ' ' << v.z;
        return strm;
    }

    TriangleMesh3::TriangleMesh3(const Transform3& transform_, bool IsNormalFlipped_)
                    : Surface3(transform_, IsNormalFlipped_)
    {}


    TriangleMesh3::TriangleMesh3(const PointArray& Points, const NormalArray& Normals,
                                const UVArray& UVs, const IndexArray& PointIndices,
                                const IndexArray& NormalIndices, const IndexArray& UVIndices,
                                const Transform3& transform_, bool IsNormalFlipped_)
                                : Surface3(transform_, IsNormalFlipped_),
                                    _Points(Points), _Normals(Normals), _UVs(UVs),
                                    _PointIndices(PointIndices), _NormalIndices(NormalIndices),
                                    _UVIndices(UVIndices)
    {}


    TriangleMesh3::TriangleMesh3(const TriangleMesh3& other) : Surface3(other)
    {
        Set(other);
    }

    Vector3D TriangleMesh3::ClosestPointLocal(const Vector3D& otherPoint) const
    {
        static const double m = std::numeric_limits<double>::max();
        Vector3D MinDistPt(m,m,m);
        double MinDistSquared = m;
        
        size_t n = NumberOfTriangles();
        for(size_t i = 0; i < n; ++i)
        {
            Triangle3 tri = Triangle(i);
            Vector3D pt = tri.ClosestPoint(otherPoint);
            double distSquared = (otherPoint - pt).LengthSquared();
            if(distSquared < MinDistSquared)
            {
                MinDistSquared = distSquared;
                MinDistPt = pt;
            }
        }

        return MinDistPt;
    }

    Vector3D TriangleMesh3::ClosestNormalLocal(const Vector3D& otherPoint) const
    {
        static const double m = std::numeric_limits<double>::max();
        Vector3D MinDistPt(m,m,m);
        Vector3D MinDistNormal(1,0,0);
        double MinDistSquared = m;

        size_t n = NumberOfTriangles();
        for (size_t i = 0; i < n; ++i)
        {
            Triangle3 tri = Triangle(i);
            Vector3D pt = tri.ClosestPoint(otherPoint);
            double DistSquared = (otherPoint - pt).LengthSquared();
            if(DistSquared < MinDistSquared)
            {
                MinDistSquared = DistSquared;
                MinDistPt = pt;
                MinDistNormal = tri.ClosestNormal(otherPoint);
            }
        }
        return MinDistNormal;
    }

    SurfaceRayIntersection3 TriangleMesh3::ClosestIntersectionLocal(const Ray3D& ray) const
    {
        SurfaceRayIntersection3 intersection;

        size_t n = NumberOfTriangles();
        double t = std::numeric_limits<double>::max();
        for (size_t i = 0; i < n; ++i)
        {
            Triangle3 tri = Triangle(i);
            SurfaceRayIntersection3 tmpIntersection = tri.ClosestIntersection(ray);

            if (tmpIntersection.t < t)
            {
                t = tmpIntersection.t;
                intersection = tmpIntersection;
            }
        }

        return intersection;
    }

    BoundingBox3D TriangleMesh3::BoundingBoxLocal() const
    {
        BoundingBox3D box;
        size_t n = _PointIndices.Size();
        for(size_t i = 0; i < n; ++i)
        {
            const Point3UI& face = _PointIndices[i];
            box.Merge(_Points[face[0]]);
            box.Merge(_Points[face[1]]);
            box.Merge(_Points[face[2]]);
        }
        
        return box;
    }

    
    bool TriangleMesh3::IntersectsLocal(const Ray3D& ray) const
    {
        size_t n = NumberOfTriangles();
        for (size_t i = 0; i < n; ++i)
        {
            Triangle3 tri = Triangle(i);
            if(tri.Intersects(ray))
            {
                return true;
            }
        }
        return false;
    }


    double TriangleMesh3::ClosestDistanceLocal(const Vector3D& otherPoint) const
    {
        double MinDist = std::numeric_limits<double>::max();

        size_t n = NumberOfTriangles();

        for(size_t i = 0; i < n; ++i)
        {
            Triangle3 tri = Triangle(i);
            MinDist = std::min(MinDist, tri.ClosestDistance(otherPoint));
        }

        return MinDist;
    }


    void TriangleMesh3::Clear()
    {
        _Points.Clear();
        _Normals.Clear();
        _UVs.Clear();
        _PointIndices.Clear();
        _NormalIndices.Clear();
        _UVIndices.Clear();
    }

    void TriangleMesh3::Set(const TriangleMesh3& other)
    {
        _Points.Set(other._Points);
        _Normals.Set(other._Normals);
        _UVs.Set(other._UVs);
        _PointIndices.Set(other._PointIndices);
        _NormalIndices.Set(other._NormalIndices);
        _UVIndices.Set(other._UVIndices);
    }

    void TriangleMesh3::Swap(TriangleMesh3& other)
    {
        _Points.Swap(other._Points);
        _Normals.Swap(other._Normals);
        _UVs.Swap(other._UVs);
        _PointIndices.Swap(other._PointIndices);
        _NormalIndices.Swap(other._NormalIndices);
        _UVIndices.Swap(other._UVIndices);
    }

    double TriangleMesh3::Area() const
    {
        double a = 0;
        for (size_t i = 0; i < NumberOfTriangles(); ++i)
        {
            Triangle3 tri = Triangle(i);
            a += tri.Area();
        }
        return a;
    }

    double TriangleMesh3::Volume() const
    {
        double vol = 0;
        for (size_t i = 0; i < NumberOfTriangles(); ++i)
        {
            Triangle3 tri = Triangle(i);
            vol += tri.Points[0].Dot(tri.Points[1].Cross(tri.Points[2])) / 6.f;
        }

        return vol;
    }

    const Vector3D& TriangleMesh3::Point(size_t i) const
    {
        return _Points[i];
    }

    Vector3D& TriangleMesh3::Point(size_t i)
    {
        return _Points[i];
    }

    const Vector3D& TriangleMesh3::Normal(size_t i) const
    {
        return _Normals[i];
    }

    Vector3D& TriangleMesh3::Normal(size_t i)
    {
        return _Normals[i];
    }

    const Vector2D& TriangleMesh3::UV(size_t i) const
    {
        return _UVs[i];
    }

    Vector2D& TriangleMesh3::UV(size_t i)
    {
        return _UVs[i];
    }


    const Point3UI& TriangleMesh3::PointIndex(size_t i) const
    {
        return _PointIndices[i];
    }


    Point3UI& TriangleMesh3::PointIndex(size_t i)
    {
        return _PointIndices[i];
    }

    const Point3UI& TriangleMesh3::NormalIndex(size_t i) const
    {
        return _NormalIndices[i];
    }

    Point3UI& TriangleMesh3::NormalIndex(size_t i)
    {
        return _NormalIndices[i];
    }

    const Point3UI& TriangleMesh3::UVIndex(size_t i) const
    {
        return _UVIndices[i];
    }

    Point3UI& TriangleMesh3::UVIndex(size_t i)
    {
        return _UVIndices[i];
    }

    Triangle3 TriangleMesh3::Triangle(size_t i) const
    {
        Triangle3 tri;
        for (int j = 0; j < 3; ++j)
        {
            tri.Points[j] = _Points[_PointIndices[i][j]];
            if(HasUVs())
            {
                tri.UVs[j] = _UVs[_UVIndices[i][j]];
            }
        }

        Vector3D n = tri.FaceNormal();

        for (int j = 0; j < 3; ++j)
        {
            if (HasNormals())
            {
                tri.Normals[j] = _Normals[_NormalIndices[i][j]];
            }
            else
            {
                tri.Normals[j] = n;
            }
        }

        return tri;
    }

    
    size_t TriangleMesh3::NumberOfPoints() const
    {
        return _Points.Size();
    }


    size_t TriangleMesh3::NumberOfNormals() const
    {
        return _Normals.Size();
    }

    size_t TriangleMesh3::NumberOfUVs() const
    {
        return _UVs.Size();
    }

    size_t TriangleMesh3::NumberOfTriangles() const
    {
        return _PointIndices.Size();
    }

    bool TriangleMesh3::HasNormals() const
    {
        return _Normals.Size() > 0;
    }

    bool TriangleMesh3::HasUVs() const
    {
        return _UVs.Size() > 0;
    }

    void TriangleMesh3::AddPoint(const Vector3D& pt)
    {
        _Points.Append(pt);
    }

    void TriangleMesh3::AddNormal(const Vector3D& n)
    {
        _Normals.Append(n);
    }

    void TriangleMesh3::AddUV(const Vector2D& t)
    {
        _UVs.Append(t);
    }

    void TriangleMesh3::AddPointTriangle(const Point3UI& NewPointIndices)
    {
        _PointIndices.Append(NewPointIndices);
    }

    void TriangleMesh3::AddPointNormalTriangle(const Point3UI& NewPointIndices, const Point3UI& NewNormalIndices)
    {
        //Number of normal indices must match with number of point indices and number of UVs.
        JET_ASSERT(_PointIndices.Size() == _NormalIndices.Size());

        _PointIndices.Append(NewPointIndices);
        _NormalIndices.Append(NewNormalIndices);
    }


    void TriangleMesh3::AddPointUVNormalTriangle(const Point3UI& NewPointIndices,
                                                const Point3UI& NewUVIndices,
                                                const Point3UI& NewNormalIndices)
    {
        //Number of normal indices must match with number of point indices and UVs.
        JET_ASSERT(_PointIndices.Size() == _NormalIndices.Size());
        JET_ASSERT(_PointIndices.Size() == _UVs.Size());

        _PointIndices.Append(NewPointIndices);
        _NormalIndices.Append(NewNormalIndices);
        _UVIndices.Append(NewUVIndices);
    }

    void TriangleMesh3::AddPointUVTriangle(const Point3UI& NewPointIndices, const Point3UI& NewUVIndices)
    {
        // Number of normal indices must match with normal indices and UVs.
        JET_ASSERT(_PointIndices.Size() == _UVs.Size());
        _PointIndices.Append(NewPointIndices);
        _UVIndices.Append(NewUVIndices);
    }

    
    void TriangleMesh3::AddTriangle(const Triangle3& tri)
    {
        size_t vStart = _Points.Size();
        size_t nStart = _Normals.Size();
        size_t tStart = _UVs.Size();
        Point3UI NewPointIndices;
        Point3UI NewNormalIndices;
        Point3UI NewUVIndices;

        for(size_t i = 0; i < 3; ++i)
        {
            _Points.Append(tri.Points[i]);
            _Normals.Append(tri.Normals[i]);
            _UVs.Append(tri.UVs[i]);
            NewPointIndices[i] = vStart + 1;
            NewNormalIndices[i] = nStart + 1;
            NewUVIndices[i] = tStart + 1;
        }

        _PointIndices.Append(NewPointIndices);
        _NormalIndices.Append(NewNormalIndices);
        _UVIndices.Append(NewUVIndices);
    }

    void TriangleMesh3::SetFaceNormal()
    {
        _Normals.Resize(_Points.Size());
        _NormalIndices.Set(_PointIndices);

        for (size_t i = 0; i < NumberOfTriangles(); ++i)
        {
            Triangle3 tri = Triangle(i);
            Vector3D n = tri.FaceNormal();
            Point3UI f = _PointIndices[i];
            _Normals[f.x] = n;
            _Normals[f.y] = n;
            _Normals[f.z] = n;
        }
    }

    void TriangleMesh3::SetAngleWeightedVertexNormal()
    {
        _Normals.Clear();
        _NormalIndices.Clear();

        Array1<double> AngleWeights(_Points.Size());
        Vector3DArray PseudoNormals(_Points.Size());

        for(size_t i = 0; i< _Points.Size(); ++i)
        {
            AngleWeights[i] = 0;
            PseudoNormals[i] = Vector3D();
        }

        for(size_t i = 0; i < _Points.Size(); ++i)
        {
            Vector3D pts[3];
            Vector3D normal, e0, e1;
            double cosangle, angle;
            size_t idx[3];

            for (int j = 0; j < 3; ++j)
            {
                idx[j] = _PointIndices[i][j];
                pts[j] = _Points[idx[j]];
            }

            //Angle for point 0
            e0 = pts[1] - pts[0];
            e1 = pts[2] - pts[0];
            e0.Normalize();
            e1.Normalize();
            normal = e0.Cross(e1);
            normal.Normalize();
            cosangle = Clamp(e0.Dot(e1), -1.0, 1.0);
            angle = std::acos(cosangle);
            AngleWeights[idx[0]] += angle;
            PseudoNormals[idx[0]] += angle * normal;

            //Angle for point 1
            e0 = pts[2] - pts[1];
            e1 = pts[0] - pts[1];
            e0.Normalize();
            e1.Normalize();
            normal = e0.Cross(e1);
            normal.Normalize();
            cosangle = Clamp(e0.Dot(e1), -1.0, 1.0);
            angle = std::acos(cosangle);
            AngleWeights[idx[1]] += angle;
            PseudoNormals[idx[1]] += angle * normal;
            

            //Angle for point 2
            e0 = pts[0] - pts[2];
            e1 = pts[1] - pts[2];
            e0.Normalize();
            e1.Normalize();
            normal = e0.Cross(e1);
            normal.Normalize();
            cosangle = Clamp(e0.Dot(e1), -1.0, 1.0);
            angle = std::acos(cosangle);
            AngleWeights[idx[2]] += angle;
            PseudoNormals[idx[2]] += angle * normal;
        }

        for (size_t i = 0; i<_Points.Size(); ++i)
        {
            if (AngleWeights[i] > 0)
                PseudoNormals[i] /= AngleWeights[i];
        }

        std::swap(PseudoNormals, _Normals);
        _NormalIndices.Set(_PointIndices);
    }


    void TriangleMesh3::Scale(double factor)
    {
        ParallelFor(kZeroSize, NumberOfPoints(), 
                    [this, factor](size_t i)
                    {
                        _Points[i] *= factor;
                    });
    }

    void TriangleMesh3::Translate(const Vector3D& t)
    {
        ParallelFor(kZeroSize, NumberOfPoints(),
                    [this, t](size_t i){
                        _Points[i] += t;
                    });
    }

    void TriangleMesh3::Rotate(const Quaternion<double>& q)
    {
        ParallelFor(kZeroSize, NumberOfPoints(),
                    [this, q](size_t i){
                        _Points[i] = q * _Points[i];
                    });
        
        ParallelFor(kZeroSize, NumberOfNormals(),
                    [this,q](size_t i){
                        _Normals[i] = q * _Normals[i];
                    });
    }

    
    void TriangleMesh3::WriteObj(std::ostream* strm) const
    {
        //Vertex
        for (const auto& pt : _Points)
            (*strm) << "v " << pt << std::endl;

        //UV coords
        for (const auto& uv : _UVs)
            (*strm) << "vt " << uv << std::endl;

        //Normals
        for (const auto& n : _Normals)
            (*strm) << "vn " << n << std::endl;
        
        //Faces
        bool hasUVs = HasUVs();
        bool hasNormals = HasNormals();
        for (size_t i = 0; i < NumberOfTriangles(); ++i)
        {
            (*strm) << "f ";
            for (int j = 0; j < 3; ++j)
            {
                (*strm) << _PointIndices[i][j] + 1;
                if (hasNormals || hasUVs)
                    (*strm) << '/';
                if(hasUVs)
                    (*strm) << _UVIndices[i][j] + 1;
                if (hasNormals)
                    (*strm) << '/' << _NormalIndices[i][j] + 1;
                (*strm) << ' ';
            }
            (*strm) << ' ';
        }
    }

    bool TriangleMesh3::ReadObj(std::istream* strm)
    {
        obj::obj_parser parser(obj::obj_parser::triangulate_faces | 
                    obj::obj_parser::translate_negative_indices);

        
        parser.info_callback([](size_t LineNumber, const std::string& message){
            std::cout<< LineNumber << " " << message << std::endl;
        });

        parser.warning_callback([](size_t LineNumber, const std::string& message){
            std::cerr << LineNumber << " " << message << std::endl;
        });

        parser.error_callback([](size_t LineNumber, const std::string& message){
            std::cerr << LineNumber << " " << message << std::endl;
        });

        parser.geometric_vertex_callback(
            [this](obj::float_type x, obj::float_type y, obj::float_type z){
                AddPoint({x,y,z});
        });


        parser.texture_vertex_callback(
            [this](obj::float_type u, obj::float_type v){
                AddUV({u, v});
        });


        parser.vertex_normal_callback(
            [this](obj::float_type nx, obj::float_type ny, obj::float_type nz){
                AddNormal({nx, ny, nz});
        });


        parser.face_callbacks(
            // triangular_face_geometric_vertices_callback_type
            [this](obj::index_type v0, obj::index_type v1, obj::index_type v2) {
                AddPointTriangle({v0 - 1, v1 - 1, v2 - 1});
            },
            // triangular_face_geometric_vertices_texture_vertices_callback_type
            [this](const obj::index_2_tuple_type& v0_vt0,
            const obj::index_2_tuple_type& v1_vt1,
            const obj::index_2_tuple_type& v2_vt2) {
                AddPointUVTriangle(
                    {
                        std::get<0>(v0_vt0) - 1,
                        std::get<0>(v1_vt1) - 1,
                        std::get<0>(v2_vt2) - 1
                    },
                    {
                        std::get<1>(v0_vt0) - 1,
                        std::get<1>(v1_vt1) - 1,
                        std::get<1>(v2_vt2) - 1
                    });
            },
            // triangular_face_geometric_vertices_vertex_normals_callback_type
            [this](const obj::index_2_tuple_type& v0_vn0,
            const obj::index_2_tuple_type& v1_vn1,
            const obj::index_2_tuple_type& v2_vn2) {
                AddPointNormalTriangle(
                    {
                        std::get<0>(v0_vn0) - 1,
                        std::get<0>(v1_vn1) - 1,
                        std::get<0>(v2_vn2) - 1
                    },
                    {
                        std::get<1>(v0_vn0) - 1,
                        std::get<1>(v1_vn1) - 1,
                        std::get<1>(v2_vn2) - 1
                    });
            },
            // triangular_face_geometric_vertices_texture_vertices_vertex_normals...
            [this](const obj::index_3_tuple_type& v0_vt0_vn0,
            const obj::index_3_tuple_type& v1_vt1_vn1,
            const obj::index_3_tuple_type& v2_vt2_vn2) {
                AddPointUVNormalTriangle(
                    {
                        std::get<0>(v0_vt0_vn0) - 1,
                        std::get<0>(v1_vt1_vn1) - 1,
                        std::get<0>(v2_vt2_vn2) - 1
                    },
                    {
                        std::get<1>(v0_vt0_vn0) - 1,
                        std::get<1>(v1_vt1_vn1) - 1,
                        std::get<1>(v2_vt2_vn2) - 1
                    },
                    {
                        std::get<2>(v0_vt0_vn0) - 1,
                        std::get<2>(v1_vt1_vn1) - 1,
                        std::get<2>(v2_vt2_vn2) - 1
                    });
            },
            // quadrilateral_face_geometric_vertices_callback_type
            [](obj::index_type, obj::index_type, obj::index_type, obj::index_type) {
            },
            // quadrilateral_face_geometric_vertices_texture_vertices_callback_type
            [](const obj::index_2_tuple_type&,
            const obj::index_2_tuple_type&,
            const obj::index_2_tuple_type&,
            const obj::index_2_tuple_type&) {},
            // quadrilateral_face_geometric_vertices_vertex_normals_callback_type
            [](const obj::index_2_tuple_type&,
            const obj::index_2_tuple_type&,
            const obj::index_2_tuple_type&,
            const obj::index_2_tuple_type&) {},
            // quadrilateral_face_geometric_vertices_texture_vertices_vertex_norm...
            [](const obj::index_3_tuple_type&,
            const obj::index_3_tuple_type&,
            const obj::index_3_tuple_type&,
            const obj::index_3_tuple_type&) {},
            // polygonal_face_geometric_vertices_begin_callback_type
            [](obj::index_type, obj::index_type, obj::index_type) {},
            // polygonal_face_geometric_vertices_vertex_callback_type
            [](obj::index_type) {},
            // polygonal_face_geometric_vertices_end_callback_type
            []() {},
            // polygonal_face_geometric_vertices_texture_vertices_begin_callback_...
            [](const obj::index_2_tuple_type&,
            const obj::index_2_tuple_type&,
            const obj::index_2_tuple_type&) {},
            // polygonal_face_geometric_vertices_texture_vertices_vertex_callback...
            [](const obj::index_2_tuple_type&) {},
            // polygonal_face_geometric_vertices_texture_vertices_end_callback_type
            []() {},
            // polygonal_face_geometric_vertices_vertex_normals_begin_callback_type
            [](const obj::index_2_tuple_type&,
            const obj::index_2_tuple_type&,
            const obj::index_2_tuple_type&) {},
            // polygonal_face_geometric_vertices_vertex_normals_vertex_callback_type
            [](const obj::index_2_tuple_type&) {},
            // polygonal_face_geometric_vertices_vertex_normals_end_callback_type
            []() {},
            // polygonal_face_geometric_vertices_texture_vertices_vertex_normals_...
            [](const obj::index_3_tuple_type&,
            const obj::index_3_tuple_type&,
            const obj::index_3_tuple_type&) {},
            // polygonal_face_geometric_vertices_texture_vertices_vertex_normals_...
            [](const obj::index_3_tuple_type&) {},
            // polygonal_face_geometric_vertices_texture_vertices_vertex_normals_...
            []() {});

        parser.group_name_callback([](const std::string&) {});
        parser.smoothing_group_callback([](obj::size_type) {});
        parser.object_name_callback([](const std::string&) {});
        parser.material_library_callback([](const std::string&) {});
        parser.material_name_callback([](const std::string&) {});
        parser.comment_callback([](const std::string&) {});

        return parser.parse(*strm);
    }

    TriangleMesh3& TriangleMesh3::operator=(const TriangleMesh3& other)
    {
        Set(other);
        return *this;
    }

    TriangleMesh3::Builder TriangleMesh3::builder()
    {
        return Builder();
    }

    TriangleMesh3::Builder& TriangleMesh3::Builder::WithPoints(const PointArray& points)
    {
        _Points = points;
        return *this;
    }


    TriangleMesh3::Builder& TriangleMesh3::Builder::WithNormals(const NormalArray& normals)
    {
        _Normals = normals;
        return *this;
    }

    TriangleMesh3::Builder& TriangleMesh3::Builder::WithUVs(const UVArray& uvs)
    {
        _UVs = uvs;
        return *this;
    }

    TriangleMesh3::Builder& TriangleMesh3::Builder::WithPointIndices(const IndexArray& pointIndices)
    {
        _PointIndices = pointIndices;
        return *this;
    }

    TriangleMesh3::Builder& TriangleMesh3::Builder::WithNormalIndices(const IndexArray& normalIndices)
    {
        _NormalIndices = normalIndices;
        return *this;
    }

    TriangleMesh3::Builder& TriangleMesh3::Builder::WithUVIndices(const IndexArray& UVIndices)
    {
        _UVIndices = UVIndices;
        return *this;
    }

    TriangleMesh3 TriangleMesh3::Builder::Build() const
    {
        return TriangleMesh3(_Points, _Normals, _UVs,
                            _PointIndices, _NormalIndices, _UVIndices,
                            _transform, _IsNormalFlipped);
    }


    TriangleMesh3Ptr TriangleMesh3::Builder::MakeShared() const
    {
        return std::shared_ptr<TriangleMesh3>(
                    new TriangleMesh3(_Points, _Normals, _UVs,
                            _PointIndices, _NormalIndices, _UVIndices,
                            _transform, _IsNormalFlipped),
                            [](TriangleMesh3* obj){
                                delete obj;
        });
    }

}