#pragma once

#include<Vector/vector2.h>
#include<Geometry/Ray/ray.h>

namespace jet
{
    //! \brief Class for 2D ray
    //!
    //! \tparam T The value type
    template<typename T>
    class Ray<T,2> final
    {
    public:
        static_assert(std::is_floating_point<T>::value, "Ray only can only be instantiated with floating point types");

        //! Origin of the ray.
        Vector2<T> Origin;

        //! Direction of the ray.
        Vector2<T> Direction;

        //! Constructs an empty ray that points to (1,0) from (0,0)
        Ray();

        //! Constructs a ray with given origin and direction.
        Ray(const Vector2<T>& newOrigin, const Vector2<T>& newDirection);

        //! Copy Constructor
        Ray(const Ray& other);

        //! Returns a point on the ray at distance \p t.
        Vector2<T> PointAt(T t) const;
    };

    template<typename T> using Ray2 = Ray<T,2>;

    typedef Ray2<float> Ray2F;

    typedef Ray2<double> Ray2D;

    template<typename T>
    Ray<T,2>::Ray() : Ray(Vector2<T>(), Vector2<T>(1,0))
    {}

    template<typename T>
    Ray<T,2>::Ray(const Vector2<T>& newOrigin, const Vector2<T>& newDirection) :
        Origin(newOrigin), Direction(newDirection.Normalized())
    {}

    template<typename T>
    Ray<T,2>::Ray(const Ray& other) :
        Origin(other.Origin),
        Direction(other.Direction)
    {}

    template<typename T>
    Vector2<T> Ray<T,2>::PointAt(T t) const
    {
        return Origin + t * Direction;
    }
}
