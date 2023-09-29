#pragma once

#include<Geometry/BoundingBox/bounding_box2.h>
#include<Geometry/Ray/ray2.h>
#include<Vector/vector2.h>

#include<algorithm>
#include<cmath>

 namespace jet
 {
    //! \brief Represents 2D rigid body transform.

    class Transform2
    {
    public:
        //! Constructs Identity Transform
        Transform2();

        //! Constructs a transform with translation and orientation.
        Transform2(const Vector2D& translation, double orientation);

        //! Returns the translation.
        const Vector2D& Translation() const;

        //! Sets the translation.
        void SetTranslation(const Vector2D& translation);

        //! Returns the orientation in radians.
        double Orientation() const;

        //! Sets the orientation in radians.
        void SetOrientation(double orientation);

        //! Transforms a point in world coordinate to the local frame.
        Vector2D ToLocal(const Vector2D& PointInWorld) const;

        //! Transforms a direction in work coordiante to the local frame.
        Vector2D ToLocalDirection(const Vector2D& DirInWorld) const;

        //! transforms a ray in from world coordinate to the local frame.
        Ray2D ToLocal(const Ray2D& RayInWorld) const;

        //! Transforms a bounding box from world coordinate to the local frame.
        BoundingBox2D ToLocal(const BoundingBox2D& BBoxInWorld) const;

        //! Transforms a point from local space to the world coordinates.
        Vector2D ToWorld(const Vector2D& PointInLocal) const;

        //! Transforms a direction from local space to the world coordinates.
        Vector2D ToWorldDirection(const Vector2D& DirInLocal) const;

        //! Transforms a ray from local space to the world coordinates.
        Ray2D ToWorld(const Ray2D& RayInLocal) const;

        //! Transforms a bounding box from local space to the world coordinates.
        BoundingBox2D ToWorld(const BoundingBox2D& BBoxInLocal) const;
    private:
        Vector2D _translation;
        double _orientation = 0.0;
        double _cosAngle = 1.0;
        double _sinAngle = 0.0;
    };

    inline Transform2::Transform2()
    {}

    inline Transform2::Transform2(const Vector2D& translation, double orientation)
            : _translation(translation), _orientation(orientation)
            {
                _cosAngle = std::cos(orientation);
                _sinAngle = std::sin(orientation);
            }

    
    inline const Vector2D& Transform2::Translation() const{
        return _translation;
    }

    inline void Transform2::SetTranslation(const Vector2D& translation)
    {
        _translation = translation;
    }

    inline double Transform2::Orientation() const
    {
        return _orientation;
    }

    inline void Transform2::SetOrientation(double orientation)
    {
        _orientation = orientation;
        _cosAngle = std::cos(orientation);
        _sinAngle = std::sin(orientation);
    }

    inline Vector2D Transform2::ToLocal(const Vector2D& PointInWorld) const
    {
        Vector2D xmt = PointInWorld - _translation;
        return Vector2D(
            _cosAngle * xmt.x + _sinAngle * xmt.y,
            -_sinAngle * xmt.x + _cosAngle * xmt.y);
    }

    inline Vector2D Transform2::ToLocalDirection(const Vector2D& DirInWorld) const
    {
        return Vector2D(
            _cosAngle * DirInWorld.x + _sinAngle * DirInWorld.y,
            -_sinAngle * DirInWorld.x + _cosAngle * DirInWorld.y);
    }

    inline Ray2D Transform2::ToLocal(const Ray2D& RayInWorld) const
    {
        return Ray2D(
            ToLocal(RayInWorld.Origin), ToLocalDirection(RayInWorld.Direction));
    }

    inline BoundingBox2D Transform2::ToLocal(const BoundingBox2D& BBoxInWorld) const
    {
        BoundingBox2D BBoxInLocal = BBoxInWorld;
        for(int i = 0; i < 4; ++i)
        {
            auto CornerInLocal = ToLocal(BBoxInWorld.Corner(i));
            BBoxInLocal.LowerCorner = Min(BBoxInLocal.LowerCorner, CornerInLocal);
            BBoxInLocal.UpperCorner = Max(BBoxInLocal.UpperCorner, CornerInLocal);
        }
        return BBoxInLocal;
    }

    inline Vector2D Transform2::ToWorld(const Vector2D& PointInLocal) const
    {
        return Vector2D(
            _cosAngle * PointInLocal.x - _sinAngle * PointInLocal.y + _translation.x,
            _sinAngle * PointInLocal.x + _cosAngle * PointInLocal.y + _translation.y);
    }

    inline Vector2D Transform2::ToWorldDirection(const Vector2D& DirInLocal) const
    {
        return Vector2D(
            _cosAngle * DirInLocal.x - _sinAngle * DirInLocal.y,
            _sinAngle * DirInLocal.x + _cosAngle * DirInLocal.y);
    }

    inline Ray2D Transform2::ToWorld(const Ray2D& RayInLocal) const
    {
        return Ray2D(
            ToWorld(RayInLocal.Origin), ToWorldDirection(RayInLocal.Direction));
    }

    inline BoundingBox2D Transform2::ToWorld(const BoundingBox2D& BBoxInLocal) const
    {
        BoundingBox2D BBoxInWorld = BBoxInLocal;
        for(int i = 0; i < 4; ++i)
        {
            auto CornerInWorld = ToWorld(BBoxInLocal.Corner(i));
            BBoxInWorld.LowerCorner = Min(BBoxInWorld.LowerCorner, CornerInWorld);
            BBoxInWorld.UpperCorner = Max(BBoxInWorld.UpperCorner, CornerInWorld);
        }

        return BBoxInWorld;
    }

 }

