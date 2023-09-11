#pragma once

#include"bounding_box.h"
#include<Vector/vector2.h>
#include<Geometry/Ray/ray2.h>
#include<limits>

#include<algorithm>
#include<utility>

namespace jet
{
    //! \brief 2D Box-Ray intersection result.
    //!
    //! \tparam T - The value type
    template<typename T>
    struct BoundingBoxRayIntersection2
    {
        //! true if the ray intersects with the box
        bool IsIntersecting = false;

        //! Distance to the first intersection point.
        T tNear = std::numeric_limits<T>::max();

        //! Distance to the second intersection point.
        T tFar = std::numeric_limits<T>::max();
    };


    //! \brief 2D Axis-aligned bounding box class.
    //!
    //! \tparam T - Real Number type.
    //! \tparam N - Dimension
    template<typename T>
    class BoundingBox<T,2>
    {
    public:
        //! Lower Corner of the bounding box
        Vector2<T> LowerCorner;

        //! Upper corner of the bounding box
        Vector2<T> UpperCorner;

        //! Default constructor.
        BoundingBox();

        //! Constructs a box that tightly covers two points.
        BoundingBox(const Vector2<T>& point1, const Vector2<T>& point2);

        //! Constructs a box with other box instance.
        BoundingBox(const BoundingBox& other);

        //! Returns the width of the box
        T Width() const;

        //! Returns the height of the box.
        T Height() const;

        //! Returns the length of the box along a given axis.
        T Length(size_t axis);

        //! Returns true of this box and other box overlaps.
        bool Overlaps(const BoundingBox& other) const;

        //! Returns true if the input point is inside of this box.
        bool Contains(const Vector2<T>& point) const;

        //! Returns true if the input ray is intersecting with this box.
        bool Intersects(const Ray2<T>& ray) const;

        //! Returns intersection.IsIntersecting = true if the input ray is
        //! intersecting with this box.
        BoundingBoxRayIntersection2<T> ClosestIntersection(const Ray2<T>& ray) const;

        //! Returns the mid point of the box.
        Vector2<T> MidPoint() const;

        //! Returns the diagonal length of the box.
        T DiagonalLength() const;

        //! Returns Squared Diagonal Length of the box.
        T DiagonalLengthSquared() const;

        //! Resets the box to initial state (from -infinite to infinite)
        void Reset();

        //! Merges this and \p other point.
        void Merge(const Vector2<T>& point);

        //! Merges this bounding box with the bounding box \p other
        void Merge(const BoundingBox& other);

        //! Expands the box b y the given \p delta in all directions.
        void Expand(T delta);

        //! Returns corner position. Index Starts from x-first order
        Vector2<T> Corner(size_t idx) const;
    };

    template<typename T> using BoundingBox2 = BoundingBox<T,2>;

    typedef BoundingBox2<float> BoundingBox2F;

    typedef BoundingBox2<double> BoundingBox2D;

    typedef BoundingBoxRayIntersection2<float> BoundingBoxRayIntersection2F;

    typedef BoundingBoxRayIntersection2<double> BoundingBoxRayIntersection2D;


    template <typename T>
    BoundingBox<T, 2>::BoundingBox() {
        Reset();
    }

    template <typename T>
    BoundingBox<T, 2>::BoundingBox(
        const Vector2<T>& point1, const Vector2<T>& point2) {
        LowerCorner.x = std::min(point1.x, point2.x);
        LowerCorner.y = std::min(point1.y, point2.y);
        UpperCorner.x = std::max(point1.x, point2.x);
        UpperCorner.y = std::max(point1.y, point2.y);
    }

    template <typename T>
    BoundingBox<T, 2>::BoundingBox(const BoundingBox& other) :
        LowerCorner(other.LowerCorner),
        UpperCorner(other.UpperCorner) {
    }

    template <typename T>
    T BoundingBox<T, 2>::Width() const {
        return UpperCorner.x - LowerCorner.x;
    }

    template <typename T>
    T BoundingBox<T, 2>::Height() const {
        return UpperCorner.y - LowerCorner.y;
    }

    template <typename T>
    T BoundingBox<T, 2>::Length(size_t axis) {
        return UpperCorner[axis] - LowerCorner[axis];
    }

    template <typename T>
    bool BoundingBox<T, 2>::Overlaps(const BoundingBox& other) const {
        if (UpperCorner.x < other.LowerCorner.x
            || LowerCorner.x > other.UpperCorner.x) {
            return false;
        }

        if (UpperCorner.y < other.LowerCorner.y
            || LowerCorner.y > other.UpperCorner.y) {
            return false;
        }

        return true;
    }

    template <typename T>
    bool BoundingBox<T, 2>::Contains(const Vector2<T>& point) const {
        if (UpperCorner.x < point.x || LowerCorner.x > point.x) {
            return false;
        }

        if (UpperCorner.y < point.y || LowerCorner.y > point.y) {
            return false;
        }

        return true;
    }

    template <typename T>
    bool BoundingBox<T, 2>::Intersects(const Ray2<T>& ray) const {
        T tMin = 0;
        T tMax = std::numeric_limits<T>::max();

        const Vector2<T>& rayInvDir = ray.Direction.RDiv(1);

        for (int i = 0; i < 2; ++i) {
            T tNear = (LowerCorner[i] - ray.Origin[i]) * rayInvDir[i];
            T tFar = (UpperCorner[i] - ray.Origin[i]) * rayInvDir[i];

            if (tNear > tFar) {
                std::swap(tNear, tFar);
            }

            tMin = std::max(tNear, tMin);
            tMax = std::min(tFar, tMax);

            if (tMin > tMax) {
                return false;
            }
        }

        return true;
    }


    template <typename T>
    BoundingBoxRayIntersection2<T> BoundingBox<T, 2>::ClosestIntersection(
        const Ray2<T>& ray) const {
        BoundingBoxRayIntersection2<T> intersection;

        T tMin = 0;
        T tMax = std::numeric_limits<T>::max();

        const Vector2<T>& rayInvDir = ray.Direction.RDiv(1);

        for (int i = 0; i < 2; ++i) {
            T tNear = (LowerCorner[i] - ray.Origin[i]) * rayInvDir[i];
            T tFar = (UpperCorner[i] - ray.Origin[i]) * rayInvDir[i];

            if (tNear > tFar) {
                std::swap(tNear, tFar);
            }

            tMin = std::max(tNear, tMin);
            tMax = std::min(tFar, tMax);

            if (tMin > tMax) {
                intersection.IsIntersecting = false;
                return intersection;
            }
        }

        intersection.IsIntersecting = true;

        if (Contains(ray.Origin)) {
            intersection.tNear = tMax;
            intersection.tFar = std::numeric_limits<T>::max();
        } else {
            intersection.tNear = tMin;
            intersection.tFar = tMax;
        }

        return intersection;
    }

    template <typename T>
    Vector2<T> BoundingBox<T, 2>::MidPoint() const {
        return (UpperCorner + LowerCorner) / static_cast<T>(2);
    }

    template <typename T>
    T BoundingBox<T, 2>::DiagonalLength() const {
        return (UpperCorner - LowerCorner).Length();
    }

    template <typename T>
    T BoundingBox<T, 2>::DiagonalLengthSquared() const {
        return (UpperCorner - LowerCorner).LengthSquared();
    }

    template <typename T>
    void BoundingBox<T, 2>::Reset() {
        LowerCorner.x = std::numeric_limits<T>::max();
        LowerCorner.y = std::numeric_limits<T>::max();
        UpperCorner.x = -std::numeric_limits<T>::max();
        UpperCorner.y = -std::numeric_limits<T>::max();
    }

    template <typename T>
    void BoundingBox<T, 2>::Merge(const Vector2<T>& point) {
        LowerCorner.x = std::min(LowerCorner.x, point.x);
        LowerCorner.y = std::min(LowerCorner.y, point.y);
        UpperCorner.x = std::max(UpperCorner.x, point.x);
        UpperCorner.y = std::max(UpperCorner.y, point.y);
    }

    template <typename T>
    void BoundingBox<T, 2>::Merge(const BoundingBox& other) {
        LowerCorner.x = std::min(LowerCorner.x, other.LowerCorner.x);
        LowerCorner.y = std::min(LowerCorner.y, other.LowerCorner.y);
        UpperCorner.x = std::max(UpperCorner.x, other.UpperCorner.x);
        UpperCorner.y = std::max(UpperCorner.y, other.UpperCorner.y);
    }

    template <typename T>
    void BoundingBox<T, 2>::Expand(T delta) {
        LowerCorner -= delta;
        UpperCorner += delta;
    }

    template <typename T>
    Vector2<T> BoundingBox<T, 2>::Corner(size_t idx) const {
        Vector2<T> result;
        if (idx & 1) {
            result.x = UpperCorner.x;
        } else {
            result.x = LowerCorner.x;
        }
        if (idx & 2) {
            result.y = UpperCorner.y;
        } else {
            result.y = LowerCorner.y;
        }
        return result;
    }
} 