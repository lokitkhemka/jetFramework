#pragma once

#include<Vector/vector.h>

#include<math-utils.h>
#include<algorithm>
#include<limits>

namespace jet
{
    //! \brief Generic N-Dimensional axis-aligned Bounding Box class.
    //!
    //! \tparam T - Real Number type
    //! \tparam N - Dimension

    template<typename T, size_t N>
    class  BoundingBox
    {
    public:
        static_assert( N > 0, "Size of the bounding box should be greater than zero.");

        typedef Vector<T,N> VectorType;

        //! Lower Corner of the bounding box.
        VectorType LowerCorner;

        //! Upper Corner of the bounding box.
        VectorType UpperCorner;

        //! Default constructor
        BoundingBox();

        //! Constructs a Box that covers two points.
        BoundingBox(const VectorType& point1, const VectorType& point2);

        //! Constructs a box with other box instance.
        BoundingBox(const BoundingBox& other);

        //! Returns true if box this and \p other box overlaps.
        bool Overlaps(const BoundingBox& other) const;

        //! Returns true if the input \p point is inside of this box.
        bool Contains(const VectorType& point) const;

        //! Returns the mid point of the box.
        VectorType MidPoint() const;

        //! Returns the diagonal length of the box.
        T DiagonalLength() const;

        //! Returns Squared Diagonal Length of the box.
        T DiagonalLengthSquared() const;

        
        //! Resets the box to initial state (from -infinite to infinite)
        void Reset();

        //! Merges this and \p other point.
        void Merge(const VectorType& point);

        //! Merges this bounding box with the bounding box \p other
        void Merge(const BoundingBox& other);

        //! Expands the box b y the given \p delta in all directions.
        void Expand(T delta);
    };


    template<typename T, size_t N>
    BoundingBox<T,N>::BoundingBox()
    {
        Reset();
    }

    template<typename T, size_t N>
    BoundingBox<T,N>::BoundingBox(const VectorType& point1, const VectorType& point2)
    {
        for (size_t i = 0; i < N; ++i)
        {
            LowerCorner[i] = std::min(point1[i], point2[i]);
            UpperCorner[i] = std::max(point1[i], point2[i]);
        }
    }

    template <typename T, size_t N>
    BoundingBox<T, N>::BoundingBox(const BoundingBox& other) :
        LowerCorner(other.LowerCorner),
        UpperCorner(other.UpperCorner) {
    }

    template <typename T, size_t N>
    bool BoundingBox<T, N>::Overlaps(const BoundingBox& other) const {
        for (size_t i = 0; i < N; ++i) {
            if (UpperCorner[i] < other.LowerCorner[i]
                || LowerCorner[i] > other.UpperCorner[i]) {
                return false;
            }
        }

        return true;
    }

    template<typename T, size_t N>
    bool BoundingBox<T,N>::Contains(const VectorType& point) const
    {
        for(size_t i = 0; i< N; ++i)
        {
            if(UpperCorner[i] < point[i] || LowerCorner[i] > point[i])
            {
                return false;
            }
        }
        return true;
    }

    template<typename T, size_t N>
    Vector<T,N> BoundingBox<T,N>::MidPoint() const
    {
        Vector<T,N> result;
        for(size_t i = 0; i < N; ++i)
        {
            result[i] = (UpperCorner[i] + LowerCorner[i]) / 2;
        }
        return result;
    }

    template <typename T, size_t N>
    T BoundingBox<T, N>::DiagonalLength() const {
        T result = 0;
        for (size_t i = 0; i < N; ++i) {
            result += Square(UpperCorner[i] - LowerCorner[i]);
        }
        return std::sqrt(result);
    }

    template <typename T, size_t N>
    T BoundingBox<T, N>::DiagonalLengthSquared() const {
        T result = 0;
        for (size_t i = 0; i < N; ++i) {
            result += Square(UpperCorner[i] - LowerCorner[i]);
        }
        return result;
    }

    template <typename T, size_t N>
    void BoundingBox<T, N>::Reset() {
        for (size_t i = 0; i < N; ++i) {
            LowerCorner[i] = std::numeric_limits<T>::max();
            UpperCorner[i] = -std::numeric_limits<T>::max();
        }
    }

    template <typename T, size_t N>
    void BoundingBox<T, N>::Merge(const VectorType& point) {
        for (size_t i = 0; i < N; ++i) {
            LowerCorner[i] = std::min(LowerCorner[i], point[i]);
            UpperCorner[i] = std::max(UpperCorner[i], point[i]);
        }
    }

    template <typename T, size_t N>
    void BoundingBox<T, N>::Merge(const BoundingBox& other) {
        for (size_t i = 0; i < N; ++i) {
            LowerCorner[i] = std::min(LowerCorner[i], other.LowerCorner[i]);
            UpperCorner[i] = std::max(UpperCorner[i], other.UpperCorner[i]);
        }
    }

    template <typename T, size_t N>
    void BoundingBox<T, N>::Expand(T delta) {
        for (size_t i = 0; i < N; ++i) {
            LowerCorner[i] -= delta;
            UpperCorner[i] += delta;
        }
    }

}