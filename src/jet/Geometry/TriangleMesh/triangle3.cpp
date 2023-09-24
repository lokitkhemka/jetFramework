#include<jet.h>
#include"triangle3.h"

namespace jet
{
    inline Vector3D ClosestPointOnLine(const Vector3D& v0, const Vector3D& v1, const Vector3D& pt)
    {
        const double LenSquared = (v1 - v0).LengthSquared();
        if (LenSquared < std::numeric_limits<double>::epsilon())
        {
            return v0;
        }

        const double t = (pt - v0).Dot(v1 - v0) / LenSquared;
        if (t < 0.0)
        {
            return v0;
        }
        else if (t > 1.0)
        {
            return v1;
        }

        return v0 + t * (v1 - v0);
    }

    inline Vector3D ClosestNormalOnLine(const Vector3D& v0, const Vector3D& v1,
                                        const Vector3D& n0, const Vector3D& n1,
                                        const Vector3D& pt)
    {
        const double LenSquared = (v1 - v0).LengthSquared();
        if(LenSquared<std::numeric_limits<double>::epsilon())
        {
            return n0;
        }

        const double t = (pt - v0).Dot(v1 - v0)/LenSquared;
        if(t < 0.0)
        {
            return n0;
        }
        else if(t > 1.0)
        {
            return n1;
        }

        return (n0 + t * (n1 - n0)).Normalized();
    }

    Triangle3::Triangle3(const Transform3& transform_, bool IsNormalFlipped_)
                : Surface3(transform_, IsNormalFlipped_)
    {}

    Triangle3::Triangle3(const std::array<Vector3D, 3>& newPoints,
                        const std::array<Vector3D, 3>& newNormals,
                        const std::array<Vector2D, 3>& newUVs,
                        const Transform3& transform_, bool IsNormalFlipped_)
                        : Surface3(transform_, IsNormalFlipped_),
                            Points(newPoints), Normals(newNormals), UVs(newUVs)
    {}

    Triangle3::Triangle3(const Triangle3& other)
                    : Surface3(other), Points(other.Points),
                        Normals(other.Normals), UVs(other.UVs)
    {}

    Vector3D Triangle3::ClosestPointLocal(const Vector3D& otherPoint) const
    {
        Vector3D n = FaceNormal();
        double nd = n.Dot(n);
        double d = n.Dot(Points[0]);
        double t = (d - n.Dot(otherPoint)) / nd;

        Vector3D q = t * n + otherPoint;

        Vector3D q01 = (Points[1] - Points[0]).Cross(q - Points[1]);
        if (n.Dot(q01) < 0)
        {
            return ClosestPointOnLine(Points[0], Points[1], q);
        }
        
        Vector3D q12 = (Points[2] - Points[1]).Cross(q - Points[1]);
        if (n.Dot(q12) < 0)
        {
            return ClosestPointOnLine(Points[1], Points[2], q);
        }
        
        Vector3D q02 = (Points[0] - Points[2]).Cross(q - Points[2]);
        if (n.Dot(q02) < 0)
        {
            return ClosestPointOnLine(Points[0], Points[2], q);
        }

        double a = Area();
        double b0 = 0.5 * q12.Length() / a;
        double b1 = 0.5 * q02.Length() / a;
        double b2 = 0.5 * q01.Length() / a;

        return b0 * Points[0] + b1 * Points[1] + b2 * Points[2];
    }

    Vector3D Triangle3::ClosestNormalLocal(const Vector3D& otherPoint) const
    {
        Vector3D n = FaceNormal();
        double nd = n.Dot(n);
        double d = n.Dot(Points[0]);
        double t = (d - n.Dot(otherPoint)) / nd;

        Vector3D q = t * n + otherPoint;

        Vector3D q01 = (Points[1] - Points[0]).Cross(q - Points[0]);
        if (n.Dot(q01) < 0)
            return ClosestNormalOnLine(Points[0], Points[1], Normals[0], Normals[1], q);
        
        Vector3D q12 = (Points[2] - Points[1]).Cross(q - Points[1]);
        if (n.Dot(q12) < 0)
            return ClosestNormalOnLine(Points[1], Points[2], Normals[1], Normals[2], q);
        
        Vector3D q02 = (Points[0] - Points[2]).Cross(q - Points[2]);
        if (n.Dot(q02) < 0)
            return ClosestNormalOnLine(Points[0], Points[2], Normals[0], Normals[2], q);
        
        double a = Area();
        double b0 = 0.5 * q12.Length() / a;
        double b1 = 0.5 * q02.Length() / a;
        double b2 = 0.5 * q01.Length() / a;

        return (b0 * Normals[0] + b1 * Normals[1] + b2 * Normals[2]).Normalized();
    }

    bool Triangle3::IntersectsLocal(const Ray3D& ray) const
    {
        Vector3D n = FaceNormal();

        double nd = n.Dot(ray.Direction);

        if(nd < std::numeric_limits<double>::epsilon())
        {
            return false;
        }

        double d = n.Dot(Points[0]);
        double t = (d - n.Dot(ray.Origin)) / nd;

        if (t<0.0)
            return false;
        
        Vector3D q = ray.PointAt(t);

        Vector3D q01 = (Points[1] - Points[0]).Cross(q - Points[0]);
        if (n.Dot(q01) <= 0.0)
            return false;
        
        Vector3D q12 = (Points[2] - Points[1]).Cross(q - Points[1]);
        if (n.Dot(q12) <= 0.0)
            return false;
        
        Vector3D q02 = (Points[0] - Points[2]).Cross(q - Points[2]);
        if (n.Dot(q02) <= 0.0)
            return false;
        
        return true;
    }

    SurfaceRayIntersection3 Triangle3::ClosestIntersectionLocal(const Ray3D& ray) const
    {
        SurfaceRayIntersection3 intersection;
        Vector3D n = FaceNormal();
        double nd = n.Dot(ray.Direction);

        if (nd < std::numeric_limits<double>::epsilon())
        {
            intersection.IsIntersecting = false;
            return intersection;
        }

        double d = n.Dot(Points[0]);
        double t = (d - n.Dot(ray.Origin)) / nd;

        if (t < 0.0)
        {
            intersection.IsIntersecting = false;
            return intersection;
        }

        Vector3D q = ray.PointAt(t);

        Vector3D q01 = (Points[1] - Points[0]).Cross(q - Points[0]);
        if (n.Dot(q01) <= 0.0)
        {
            intersection.IsIntersecting = false;
            return intersection;
        }
        
        Vector3D q12 = (Points[2] - Points[1]).Cross(q - Points[1]);
        if (n.Dot(q12) <= 0.0)
        {
            intersection.IsIntersecting = false;
            return intersection;
        }
        
        Vector3D q02 = (Points[0] - Points[2]).Cross(q - Points[2]);
        if (n.Dot(q02) <= 0.0)
        {
            intersection.IsIntersecting = false;
            return intersection;
        }

        double a = Area();
        double b0 = 0.5 * q12.Length() / a;
        double b1 = 0.5 * q02.Length() / a;
        double b2 = 0.5 * q01.Length() / a;

        Vector3D normal =  (b0 * Normals[0] + b1 * Normals[1] + b2 * Normals[2]).Normalized();

        intersection.IsIntersecting = true;
        intersection.t = t;
        intersection.Point = q;
        intersection.Normal = normal;
        return intersection;
    }

    BoundingBox3D Triangle3::BoundingBoxLocal() const
    {
        BoundingBox3D box(Points[0], Points[1]);
        box.Merge(Points[2]);
        return box;
    }

    double Triangle3::Area() const
    {
        return 0.5 * (Points[1] - Points[0]).Cross(Points[2] - Points[0]).Length();
    }

    void Triangle3::GetBarycentricCoords(const Vector3D& pt, double* b0, double* b1, double* b2) const
    {
        Vector3D q01 = (Points[1] - Points[0]).Cross(pt - Points[0]);
        Vector3D q12 = (Points[2] - Points[1]).Cross(pt - Points[1]);
        Vector3D q02 = (Points[0] - Points[2]).Cross(pt - Points[2]);

        double a = Area();
        *b0 = 0.5 * q12.Length() / a;
        *b1 = 0.5 * q02.Length() / a;
        *b2 = 0.5 * q01.Length() / a;
    }

    Vector3D Triangle3::FaceNormal() const
    {
        Vector3D ret = (Points[1] - Points[0]).Cross(Points[2] - Points[0]);
        return ret.Normalized();
    }

    void Triangle3::SetNormalsToFaceNormal()
    {
        Normals[0] = Normals[1] = Normals[2] = FaceNormal();
    }

    Triangle3::Builder Triangle3::builder()
    {
        return Builder();
    }

    Triangle3::Builder& Triangle3::Builder::WithPoints(const std::array<Vector3D, 3>& points)
    {
        _points = points;
        return *this;
    }

    Triangle3::Builder& Triangle3::Builder::WithNormals(const std::array<Vector3D, 3>& normals)
    {
        _normals = normals;
        return *this;
    }

    Triangle3::Builder& Triangle3::Builder::WithUVs(const std::array<Vector2D, 3>& uvs)
    {
        _uvs = uvs;
        return *this;
    }

    Triangle3 Triangle3::Builder::Build() const
    {
        return Triangle3(_points, _normals, _uvs,_transform, _IsNormalFlipped);
    }

    Triangle3Ptr Triangle3::Builder::MakeShared() const
    {
        return std::shared_ptr<Triangle3>(new Triangle3(_points, _normals, _uvs, _transform, _IsNormalFlipped)
                                            , [](Triangle3* obj){delete obj;});
    }



}