#pragma once

#include<Geometry/BoundingBox/bounding_box3.h>
#include<constants.h>
#include<Geometry/Ray/ray3.h>
#include<Geometry/Transform/transform3.h>
#include<memory>

namespace jet
{
    //! Struct that represents ray-surface intersection.
    struct SurfaceRayIntersection3
    {
        bool IsIntersecting = false;
        double t = kMaxD;
        Vector3D Point;
        Vector3D Normal;
    };

    //! Abstract base class for 3D surface.
    class Surface3
    {
    public:
        //! Local to World Transform.
        Transform3 transform;

        //! Flips normal when called Surface3::ClosestNormal(...)
        bool IsNormalFlipped = false;

        //! Constructs a surface with normal direction.
        Surface3(const Transform3& transform = Transform3(),
                bool isNormalFlipped = false);

        //! Copy Constructor
        Surface3(const Surface3& other);

        //! Default Destructor
        virtual ~Surface3();

        //! Returns the closest point from the given point \p otherPoint to the surface.
        Vector3D ClosestPoint(const Vector3D& otherPoint) const;

        //! Returns the bounding box of this surface object
        BoundingBox3D BoundingBox() const;

        //! Returns true fi the given \p ray intersects with this surface object.
        bool Intersects(const Ray3D& ray) const;

        //! Returns the closest distance from the given point \p otherPoint to the point on the surface.
        double ClosestDistance(const Vector3D& otherPoint) const;

        //! Returns the closest intersection point for given \p ray.
        SurfaceRayIntersection3 ClosestIntersection(const Ray3D& ray) const;

        //! Returns the nomral to the closest point on the surface from the given point \p otherPoint.
        Vector3D ClosestNormal(const Vector3D& otherPoint) const;

    protected:
        //! Returns the closest point from the given point \p otherPoint to the surface in local frame.
        virtual Vector3D ClosestPointLocal(const Vector3D& otherPoint) const = 0;

        //! Returns the bounding box of this surface object in local frame.
        virtual BoundingBox3D BoundingBoxLocal() const = 0;

        //! Returns the closest intersection point for given \p ray in local frame.
        virtual SurfaceRayIntersection3 ClosestIntersectionLocal(
            const Ray3D& ray) const = 0;

        //! Returns the normal to the closest point on the surface from the given
        //! point \p otherPoint in local frame.
        virtual Vector3D ClosestNormalLocal(const Vector3D& otherPoint) const = 0;

        //! Returns true if the given \p ray intersects with this surface object
        //! in local frame.
        virtual bool IntersectsLocal(const Ray3D& ray) const;

        //! Returns the closest distance from the given point \p otherPoint to the
        //! point on the surface in local frame.
        virtual double ClosestDistanceLocal(const Vector3D& otherPoint) const;
    };

    typedef std::shared_ptr<Surface3> Surface3Ptr;

    //! \brief Base class for 3D surface builder.

    template<typename DerivedBuilder>
    class SurfaceBuilderBase3
    {
    public:
        //! Returns builder with flipped normal flag.
        DerivedBuilder& WithIsNormalFlipped(bool IsNormalFlipped);

        //! Returns builder with translation.
        DerivedBuilder& WithTranslation(const Vector3D& translation);

        //! Returns builder with orientation
        DerivedBuilder& WithOrientation(const QuaternionD& orientation);

        //! Returns builder with transform.
        DerivedBuilder& WithTransform(const Transform3& transform);

    protected:
        bool _IsNormalFlipped = false;
        Transform3 _transform;
    };

    template <typename T>
    T& SurfaceBuilderBase3<T>::WithIsNormalFlipped(bool isNormalFlipped) {
        _IsNormalFlipped = isNormalFlipped;
        return static_cast<T&>(*this);
    }

    template <typename T>
    T& SurfaceBuilderBase3<T>::WithTranslation(const Vector3D& translation) {
        _transform.SetTranslation(translation);
        return static_cast<T&>(*this);
    }

    template <typename T>
    T& SurfaceBuilderBase3<T>::WithOrientation(const QuaternionD& orientation) {
        _transform.SetOrientation(orientation);
        return static_cast<T&>(*this);
    }

    template <typename T>
    T& SurfaceBuilderBase3<T>::WithTransform(const Transform3& transform) {
        _transform = transform;
        return static_cast<T&>(*this);
    }
    
}