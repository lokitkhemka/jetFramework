#pragma once

#include<Vector/vector3.h>
#include"ray.h"

namespace jet
{
    //! \brief Class for 3D ray.
    //!
    //! \tparam T The value type
    template<typename T>
    class Ray<T,3> final
    {
    public:
        static_assert(std::is_floating_point<T>::value, "Ray can only be instantiated with floating point types");

        //! Origin of the ray
        Vector3<T> Origin;

        //! Direction of the ray
        Vector3<T> Direction;

        //! Constructs an empty ray that points to (1,0) from (0,0)
        Ray();

        //! Constructs a ray with given origin and direction.
        Ray(const Vector3<T>& newOrigin, const Vector3<T>& newDirection);

        //! Copy Constructor
        Ray(const Ray& other);

        //! Returns a point on the ray at distance \p t.
        Vector3<T> PointAt(T t) const;
    };

    template<typename T> using Ray3 = Ray<T,3>;

    typedef Ray3<float> Ray3F;

    typedef Ray3<double> Ray3D;

    template<typename T>
    Ray<T,3>::Ray() : Ray(Vector3<T>(), Vector3<T>(1,0,0))
    {}

    template<typename T>
    Ray<T,3>::Ray(const Vector3<T>& newOrigin, const Vector3<T>& newDirection) :
        Origin(newOrigin), Direction(newDirection.Normalized())
    {}

    template<typename T>
    Ray<T,3>::Ray(const Ray& other) :
        Origin(other.Origin),
        Direction(other.Direction)
    {}

    template<typename T>
    Vector3<T> Ray<T,3>::PointAt(T t) const
    {
        return Origin + t * Direction;
    }
}