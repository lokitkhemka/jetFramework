#pragma once

#include<Geometry/BoundingBox/bounding_box.h>
#include<Vector/vector3.h>
#include<Geometry/Ray/ray3.h>

#include<limits>

namespace jet
{
    //! \brief 3D bos-ray intersection result struct
    //!
    //! \tparam T - Value Type
    
    template<typename T>
    struct BoundingBoxRayIntersection3
    {
        //! True i f the box and the ray intersects
        bool IsIntersecting = true;

        //! Distance to the first intersection point
        T tNear = std::numeric_limits<T>::max();

        //! Distance to the second intersection point.
        T tFar = std::numeric_limits<T>::max();
    };

    //! \brief 3D axis-aligned bounding box class
    //!
    //! \tparam T - Real Number type
    //! \tparam N - Dimension
    template<typename T>
    class BoundingBox<T,3>
    {
    public:
        //! Lower corner of the bounding box.
        Vector3<T> LowerCorner;

        //! Upper Corner of the bounding box.
        Vector3<T> UpperCorner;

        //! Default Constructor
        BoundingBox();

        //! Constructs a box that tightly covers two points.
        BoundingBox(const Vector3<T>& point1, const Vector3<T>& point2);

        //! Copy constructor
        BoundingBox(const BoundingBox& other);

        //! Returns the width of the box.
        T Width() const;

        //! Returns the height of the box.
        T Height() const;

        //! Returns the depth of the box.
        T Depth() const;

        //! Returns the length of the box in the given axis.
        T Length(size_t i);

        //! Returns true if this box and the box \p other overlaps.
        bool Overlaps(const BoundingBox& other) const;

        //! Returns true if the input vector is inside this box.
        bool Contains(const Vector3<T>& point) const;

        //! Returns true if the input ray is intersecting with this box.
        bool Intersects(const Ray3<T>& ray) const;

        //! Returns intersection.IsIntersecting = true if the input ray is
        //! intersecting with this box. If interesects, intersection.tNear is
        //! assigned with distant to the closest intersecting point, and
        //! intersection.tFar with furthest.
        BoundingBoxRayIntersection3<T> ClosestIntersection(
            const Ray3<T>& ray) const;

        //! Returns the mid-point of this box.
        Vector3<T> MidPoint() const;

        //! Returns diagonal length of this box.
        T DiagonalLength() const;

        //! Returns Squared Diagonal Length of this box.
        T DiagonalLengthSquared() const;

        
        //! Resets the box to initial state(min = -inf, max = inf)
        void Reset();

        //! Merges this and other point
        void Merge(const Vector3<T>& point);

        //! Merges this and other box.
        void Merge(const BoundingBox& other);

        //! Expands the box by given \p delta in all directions.
        void Expand(T delta);

        //! Returns corner position. Index starts from x-first order.
        Vector3<T> Corner(size_t idx) const;
        
    };


    template<typename T> using BoundingBox3 = BoundingBox<T,3>;

    typedef BoundingBox3<float> BoundingBox3F;

    typedef BoundingBox3<double> BoundingBox3D;

    typedef BoundingBoxRayIntersection3<float> BoundingBoxRayIntersection3F;

    typedef BoundingBoxRayIntersection3<double> BoundingBoxRayIntersection3D;



    /*
        IMPLEMENTATIONS OF ABOVE METHODS
    */
    
    template<typename T>
    BoundingBox<T,3>::BoundingBox()
    {
        Reset();
    }

    template<typename T>
    BoundingBox3<T>::BoundingBox(const Vector3<T>& point1, const Vector3<T>& point2)
    {
        LowerCorner.x = std::min(point1.x, point2.x);
        LowerCorner.y = std::min(point1.y, point2.y);
        LowerCorner.z = std::min(point1.z, point2.z);
        UpperCorner.x = std::max(point1.x, point2.x);
        UpperCorner.y = std::max(point1.y, point2.y);
        UpperCorner.z = std::max(point1.z, point2.z);
    }

    template<typename T>
    BoundingBox3<T>::BoundingBox(const BoundingBox& other)
        : LowerCorner(other.LowerCorner), UpperCorner(other.UpperCorner)
    {}

    template<typename T>
    T BoundingBox3<T>::Width() const
    {
        return UpperCorner.x - LowerCorner.x;
    }

    template<typename T>
    T BoundingBox3<T>::Height() const
    {
        return UpperCorner.y - LowerCorner.y;
    }

    template<typename T>
    T BoundingBox3<T>::Depth() const
    {
        return UpperCorner.z - LowerCorner.z;
    }

    template<typename T>
    T BoundingBox<T,3>::Length(size_t i)
    {
        return UpperCorner[i] - LowerCorner[i];
    }

    template<typename T>
    bool BoundingBox3<T>::Overlaps(const BoundingBox& other) const
    {
        if (UpperCorner.x < other.LowerCorner.x || LowerCorner.x > other.UpperCorner.x)
            return false;
        
        if (UpperCorner.y < other.LowerCorner.y || LowerCorner.y > other.UpperCorner.y)
            return false;
        
        if (UpperCorner.z < other.LowerCorner.z || LowerCorner.z > other.UpperCorner.z)
            return false;
        
        return true;
    }

    template<typename T>
    bool BoundingBox3<T>::Contains(const Vector3<T>& point) const
    {
        if (UpperCorner.x < point.x || LowerCorner.x > point.x)
            return false; 
        
        if (UpperCorner.y < point.y || LowerCorner.y > point.y)
            return false;

        if (UpperCorner.z < point.z || LowerCorner.z > point.z)
            return false;
        
        return true;
    }


    template<typename T>
    bool BoundingBox<T,3>::Intersects(const Ray3<T>& ray) const
    {
        T tMin = 0;
        T tMax = std::numeric_limits<T>::max();
        const Vector3<T>& rayInvDir = ray.Direction.RDiv(1);

        for(int i =0; i < 3; ++i)
        {
            T tNear = (LowerCorner[i] - ray.Origin[i]) * rayInvDir[i];
            T tFar = (UpperCorner[i] - ray.Origin[i]) * rayInvDir[i];

            if (tNear > tFar) std::swap(tNear, tFar);
            tMin = tNear > tMin ? tNear : tMin;
            tMax = tFar < tMax ? tFar : tMax;

            if(tMin > tMax) return false;
        }

        return true;
    }

    template <typename T>
    BoundingBoxRayIntersection3<T> BoundingBox<T, 3>::ClosestIntersection(
        const Ray3<T>& ray) const {
        BoundingBoxRayIntersection3<T> intersection;

        T tMin = 0;
        T tMax = std::numeric_limits<T>::max();

        const Vector3<T>& rayInvDir = ray.Direction.RDiv(1);

        for (int i = 0; i < 3; ++i) {
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
    Vector3<T> BoundingBox<T, 3>::MidPoint() const {
        return (UpperCorner + LowerCorner) / static_cast<T>(2);
    }

    template <typename T>
    T BoundingBox<T, 3>::DiagonalLength() const {
        return (UpperCorner - LowerCorner).Length();
    }

    template <typename T>
    T BoundingBox<T, 3>::DiagonalLengthSquared() const {
        return (UpperCorner - LowerCorner).LengthSquared();
    }

    template <typename T>
    void BoundingBox<T, 3>::Reset() {
        LowerCorner.x = std::numeric_limits<T>::max();
        LowerCorner.y = std::numeric_limits<T>::max();
        LowerCorner.z = std::numeric_limits<T>::max();
        UpperCorner.x = -std::numeric_limits<T>::max();
        UpperCorner.y = -std::numeric_limits<T>::max();
        UpperCorner.z = -std::numeric_limits<T>::max();
    }

    template <typename T>
    void BoundingBox<T, 3>::Merge(const Vector3<T>& point) {
        LowerCorner.x = std::min(LowerCorner.x, point.x);
        LowerCorner.y = std::min(LowerCorner.y, point.y);
        LowerCorner.z = std::min(LowerCorner.z, point.z);
        UpperCorner.x = std::max(UpperCorner.x, point.x);
        UpperCorner.y = std::max(UpperCorner.y, point.y);
        UpperCorner.z = std::max(UpperCorner.z, point.z);
    }

    template <typename T>
    void BoundingBox<T, 3>::Merge(const BoundingBox& other) {
        LowerCorner.x = std::min(LowerCorner.x, other.LowerCorner.x);
        LowerCorner.y = std::min(LowerCorner.y, other.LowerCorner.y);
        LowerCorner.z = std::min(LowerCorner.z, other.LowerCorner.z);
        UpperCorner.x = std::max(UpperCorner.x, other.UpperCorner.x);
        UpperCorner.y = std::max(UpperCorner.y, other.UpperCorner.y);
        UpperCorner.z = std::max(UpperCorner.z, other.UpperCorner.z);
    }

    template <typename T>
    void BoundingBox<T, 3>::Expand(T delta) {
        LowerCorner -= delta;
        UpperCorner += delta;
    }

    template <typename T>
    Vector3<T> BoundingBox<T, 3>::Corner(size_t idx) const {
        Vector3<T> result;
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
        if (idx & 4) {
            result.z = UpperCorner.z;
        } else {
            result.z = LowerCorner.z;
        }
        return result;
    }

}
