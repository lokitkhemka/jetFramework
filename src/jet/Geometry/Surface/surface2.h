#pragma once

#include<Geometry/BoundingBox/bounding_box2.h>
#include<constants.h>
#include<Geometry/Ray/ray2.h>
#include<Geometry/Transform/transform2.h>
#include<memory>

namespace jet
{
    //! Struct that represents ray-surface intersection point.
    struct SurfaceRayIntersection2
    {
        bool IsIntersecting =false;
        double t =- kMaxD;
        Vector2D Point;
        Vector2D Normal;
    };

    //! Abstract Base class for a 2D surface
    class Surface2
    {
    public:
        //! Local-to-World Transform.
        Transform2 transform;

        //! Flips normal
        bool IsNormalFlipped = false;

        //! Constructs a surface with normal direction.
        Surface2(const Transform2& transform = Transform2(), bool IsNormalFlipped = false);

        //! Copy Constructor
        Surface2(const Surface2& other);

        //! Default destructor
        virtual ~Surface2();

        //! Returns the closest point from the given point \p otherPoint to the surface.
        Vector2D ClosestPoint(const Vector2D& otherPoint) const;

        //! Returns the bounding box of the surface object.
        BoundingBox2D BoundingBox() const;

        //! Returns true if the given \p ray intersects with this surface object.
        bool Intersects(const Ray2D& ray) const;

        //! Returns the closest distance from the given point \p otherPoint to the point on the surface.
        double ClosestDistance(const Vector2D& otherPoint) const;

        //! Returns the closests intersection point for the given \p ray.
        SurfaceRayIntersection2 ClosestIntersection(const Ray2D& ray) const;

        //! Returns the normal to the closest point on the surface from the given point \p otherPoint.
        Vector2D ClosestNormal(const Vector2D& otherPoint) const;


    protected:
        //! Returns the closest point from the given point \p otherPoint to the surface in the local frame
        virtual Vector2D ClosestPointLocal(const Vector2D& otherPoint) const = 0;

        //! Returns the bounding box of this surface object in local frame.
        virtual BoundingBox2D BoundingBoxLocal() const = 0;

        //! Returns the closest intersection point for given \p ray in local frame.
        virtual SurfaceRayIntersection2 ClosestIntersectionLocal(const Ray2D& ray) const = 0;

        //! Returns the normal to the closest point on the surface from the given point \p otherPoint in local frame.
        virtual Vector2D ClosestNormalLocal(const Vector2D& otherPoint) const = 0;

        //! Returns true if the given \p ray intersects with this surface object in local frame.
        virtual bool IntersectsLocal(const Ray2D& ray) const;

        //! Returns the closest distance from the given point \p otherPoint to the point on the surface in local frame.
        virtual double ClosestDistanceLocal(const Vector2D& otherPoint) const;
    };

    typedef std::shared_ptr<Surface2> Surface2Ptr;

    //! \brief Base Class for 2D surface builder.
    template<typename DerivedBuilder>
    class SurfaceBuilderBase2
    {
    public:
        //! Returns builder with fliped normal flag.
        DerivedBuilder& WithNormalFlipped(bool IsNormalFlipped);

        //! Returns Builder with translation.
        DerivedBuilder& WithTranslation(const Vector2D& translation);

        //! Returns Builder with orientation.
        DerivedBuilder& WithOrientation(double orientation);

        //! Returns builder with transform.
        DerivedBuilder& WithTransform(const Transform2& transform);
    protected:
        bool _IsNormalFlipped = false;
        Transform2 _transform;
    };

    template<typename T>
    T& SurfaceBuilderBase2<T>::WithNormalFlipped(bool IsNormalFlipped)
    {
        _IsNormalFlipped = IsNormalFlipped;
        return static_cast<T&>(*this);
    }

    template<typename T>
    T& SurfaceBuilderBase2<T>::WithTranslation(const Vector2D& translation)
    {
        _transform.SetTranslation(translation);
        return static_cast<T&>(*this);
    }

    template<typename T>
    T& SurfaceBuilderBase2<T>::WithOrientation(double orientation)
    {
        _transform.SetOrientation(orientation);
        return static_cast<T&>(*this);
    }

    template<typename T>
    T& SurfaceBuilderBase2<T>::WithTransform(const Transform2& transform)
    {
        _transform = transform;
        return static_cast<T&>(*this);
    }
}
