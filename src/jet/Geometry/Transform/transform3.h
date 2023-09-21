#pragma once

#include<Geometry/BoundingBox/bounding_box3.h>
#include<Geometry/quaternion.h>
#include<Geometry/Ray/ray3.h>
#include<Vector/vector3.h>
#include<Matrix/matrix3.h>

#include<algorithm>
#include<cmath>

namespace jet
{
    //! \brief Represents 3D rigid body transform.

    class Transform3
    {
    public:
        //! Constructs identity transform.
        Transform3();

        //! Constructs a transform with translation and orientation.
        Transform3(const Vector3D& translation, const QuaternionD& orientation);

        //! Returns the translation.
        const Vector3D& Translation() const;

        //! Sets the translation
        void SetTranslation(const Vector3D& translation);

        //! Returns the quaternion.
        const QuaternionD& Orientation() const;

        //! Sets the orientation.
        void SetOrientation( const QuaternionD& orientation);

        //! Transforms a point in world coordinate to the local frame.
        Vector3D ToLocal(const Vector3D& PointInWorld) const;

        //! Transforms a direction in world coordinates to the local frame.
        Vector3D ToLocalDirection(const Vector3D& DirInWorld) const;

        //! Transforms a Ray in world coordinates to the local frame.
        Ray3D ToLocal(const Ray3D& RayInWorld) const;

        //! Transforms a bounding box in world coordinate to the local frame.
        BoundingBox3D ToLocal(const BoundingBox3D& BBoxInWorld) const;

        //! Transforms a point in local space to the world coordinate.
        Vector3D ToWorld(const Vector3D& PointInLocal) const;

        //! Transforms a direction in local space to the world coordinate.
        Vector3D ToWorldDirection(const Vector3D& DirInLocal) const;

        //! Transforms a ray in local space to the world coordinate.
        Ray3D ToWorld(const Ray3D& RayInLocal) const;

        //! Transforms a bounding box in local space to the world coordinate.
        BoundingBox3D ToWorld(const BoundingBox3D& BBoxInLocal) const;

    private:
        Vector3D _translation;
        QuaternionD _orientation;
        Matrix3x3D _orientationMat3;
        Matrix3x3D _inverseOrientationMat3;
    };


    inline Transform3::Transform3()
    {}

    inline Transform3::Transform3(const Vector3D& translation, const QuaternionD& orientation)
    {
        SetTranslation(translation);
        SetOrientation(orientation);
    }

    inline const Vector3D& Transform3::Translation() const
    {
        return _translation;
    }

    inline void Transform3::SetTranslation(const Vector3D& translation)
    {
        _translation = translation;
    }

    inline const QuaternionD& Transform3::Orientation() const
    {
        return _orientation;
    }

    inline void Transform3::SetOrientation(const QuaternionD& orientation)
    {
        _orientation = orientation;
        _orientationMat3 = orientation.Matrix3();
        _inverseOrientationMat3 = orientation.Inverse().Matrix3();
    }

    inline Vector3D Transform3::ToLocal(const Vector3D& PointInWorld) const
    {
        return _inverseOrientationMat3 * (PointInWorld - _translation);
    }

    inline Vector3D Transform3::ToLocalDirection(const Vector3D& DirInWorld) const
    {
        return _inverseOrientationMat3 * DirInWorld;
    }


    inline Ray3D Transform3::ToLocal(const Ray3D& RayInWorld) const
    {
        return Ray3D(
            ToLocal(RayInWorld.Origin),
            ToLocal(RayInWorld.Direction)
        );
    }

    inline BoundingBox3D Transform3::ToLocal(const BoundingBox3D& BBoxInWorld) const
    {
        BoundingBox3D BBoxInLocal = BBoxInWorld;

        for(int i = 0; i < 8; ++i)
        {
            auto cornerInLocal = ToLocal(BBoxInWorld.Corner(i));

            BBoxInLocal.LowerCorner = Min(BBoxInLocal.LowerCorner, cornerInLocal);
            BBoxInLocal.UpperCorner = Max(BBoxInLocal.UpperCorner, cornerInLocal);
        }

        return BBoxInLocal;
    }

    inline Vector3D Transform3::ToWorld(const Vector3D& pointInLocal) const {
        return (_orientationMat3 * pointInLocal) + _translation;
    }

    inline Vector3D Transform3::ToWorldDirection(
        const Vector3D& dirInLocal) const {
        return _orientationMat3 * dirInLocal;
    }

    inline Ray3D Transform3::ToWorld(const Ray3D& rayInLocal) const {
        return Ray3D(
            ToWorld(rayInLocal.Origin),
            ToWorldDirection(rayInLocal.Direction));
    }

    inline BoundingBox3D Transform3::ToWorld(
        const BoundingBox3D& bboxInLocal) const {
        BoundingBox3D bboxInWorld = bboxInLocal;
        for (int i = 0; i < 8; ++i) {
            auto cornerInWorld = ToWorld(bboxInLocal.Corner(i));
            bboxInWorld.LowerCorner
                = Min(bboxInWorld.LowerCorner, cornerInWorld);
            bboxInWorld.UpperCorner
                = Max(bboxInWorld.UpperCorner, cornerInWorld);
        }
        return bboxInWorld;
    }
}