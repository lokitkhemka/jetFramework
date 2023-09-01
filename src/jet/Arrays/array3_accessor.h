#pragma once

#include <Arrays/array_accessor.h>
#include <Points/point3.h>
#include <Size/size3.h>
#include <utility>

#include<macros.h>
#include<parallel.h>


namespace jet
{
    //!
    //! \brief 3-D Array Accessor Class.
    //!
    //! This class implements 3-D array accessor, which provides
    //! array-like data read/write functions. This class does not
    //! handle memory management.
    //! 
    //! \see Array<T,3>
    //!
    //! \tparam T - Array Value Type
    //!

    template<typename T>
    class ArrayAccessor<T, 3> final
    {
    public:
        //! Constructs empty 3D array accessor.
        ArrayAccessor();

        //!
        //! \brief Constructs an array accessor that wraps given array.
        //! \param size Size of the 3D array
        //! \param data Raw array pointer.
        ArrayAccessor( const Size3& size, T* const data);

        ArrayAccessor(size_t width, size_t height, size_t depth, T* const data);

        //! Copy Constructor
        ArrayAccessor(const ArrayAccessor& other);

        //! Replaces the content with given \p other array accessor.
        void Set(const ArrayAccessor& other);

        //! Resets the array.
        void Reset(const Size3& size, T* const data);

        //! Resets the array.
        void Reset(size_t width, size_t height, size_t depth, T* const data);

        //! Returns the reference to the \p i-th element.
        T& At(size_t i);

        //! Returns the const reference to the \p i-th element.
        const T& At(size_t i) const;

        //! Returns the reference to the element at (pt.x, pt.y, pt.z)
        T& At(const Point3UI& pt);

        //! Returns the const reference to the element at (pt.x, pt.y, pt.z)
        const T& At(const Point3UI& pt) const;

        //! Returns the reference to the element at (i,j,k)
        T& At(size_t i, size_t j, size_t k);

        //! Returns the const reference to the element at (i,j,k)
        const T& At(size_t i, size_t j, size_t k) const;

        //! Returns the begin iterator of the array.
        T* const begin() const;

        //! Returns the end iterator of the array.
        T* const end() const;

        //! Returns the begin iterator of the array.
        T* begin();

        //! Returns the end iterator of the array.
        T* end();

        //! Returns the size of the array.
        Size3 Size() const;

        //! Returns the width of the array.
        size_t Width() const;

        //! Returns the height of the array.
        size_t Height() const;

        //! Returns the depth of the array.
        size_t Depth() const;

        //! Returns the raw pointer to the array data.
        T* const Data() const;

        //! Swaps the content of with \p other array accessor.
        void Swap(ArrayAccessor& other);

        //! \brief Iterates the array and invoke given \p func for each index.
        //!
        //! This function iterates the array elements and invoke the callback
        //! function \p func. The callback function takes array's element as its
        //! input. The order of execution will be the same as the nested for-loop
        //! below:
        //!
        //! \code{.cpp}
        //! int data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
        //! ArrayAccessor<int, 3> acc(2, 3, 2, data);
        //! for(size_t k = 0; k<acc.Depth(); ++k){
        //!     for (size_t j = 0; j < acc.height(); ++j) {
        //!         for (size_t i = 0; i < acc.width(); ++i) {
        //!             func(acc(i, j));
        //!         }
        //!     }
        //! }
        //! \endcode
        //!
        //! Below is the sample usage:
        //!
        //! \code{.cpp}
        //! int data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}
        //! ArrayAccessor<int, 3> acc(2, 3, 2, data);
        //! acc.forEach([](int elem) {
        //!     printf("%d\n", elem);
        //! });
        //! \endcode
        //!
        template<typename Callback>
        void ForEach(Callback func) const;

        //! \brief Iterates the array and invoke given \p func for each index.
        //!
        //! This function iterates the array elements and invoke the callback
        //! function \p func. The callback function takes three parameters which are
        //! the (i, j, k) indices of the array. The order of execution will be the
        //! same as the nested for-loop below:
        //!
        //! \code{.cpp}
        //! int data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}
        //! ArrayAccessor<int, 3> acc(2, 3, 2, data);
        //! for (size_t k = 0; k < acc.depth(); ++k) {
        //!     for (size_t j = 0; j < acc.height(); ++j) {
        //!         for (size_t i = 0; i < acc.width(); ++i) {
        //!             func(i, j, k);
        //!         }
        //!     }
        //! }
        //! \endcode
        //!
        //! Below is the sample usage:
        //!
        //! \code{.cpp}
        //! int data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}
        //! ArrayAccessor<int, 3> acc(2, 3, 2, data);
        //! acc.ForEachIndex([&](size_t i, size_t j, size_t k) {
        //!     acc(i, j, k) = 4.f * i + 7.f * j + 3.f * k + 1.5f;
        //! });
        //! \endcode
        //!
        template <typename Callback>
        void ForEachIndex(Callback func) const;


        //! \brief Iterates the array and invoke given \p func for each index in
        //!     parallel.
        //!
        //! This function iterates the array elements and invoke the callback
        //! function \p func. The callback function takes array's element as its
        //! input. The order of execution will be non-deterministic since it runs in
        //! parallel. Below is the sample usage:
        //!
        //! \code{.cpp}
        //! int data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}
        //! ArrayAccessor<int, 3> acc(2, 3, 2, data);
        //! acc.ParallelForEach([](int& elem) {
        //!     elem *= 2;
        //! });
        //! \endcode
        //!
        //! The parameter type of the callback function doesn't have to be T&, but
        //! const T& or T can be used as well.
        //!
        template <typename Callback>
        void ParallelForEach(Callback func);

        //!
        //! \brief Iterates the array and invoke given \p func for each index in
        //!     parallel using multi-threading.
        //!
        //! This function iterates the array elements and invoke the callback
        //! function \p func in parallel using multi-threading. The callback
        //! function takes two parameters which are the (i, j, k) indices of the
        //! array. The order of execution will be non-deterministic since it runs in
        //! parallel. Below is the sample usage:
        //!
        //! \code{.cpp}
        //! int data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}
        //! ArrayAccessor<int, 3> acc(2, 3, 2, data);
        //! acc.ParallelForEachIndex([&](size_t i, size_t j, size_t k) {
        //!     acc(i, j, k) *= 2;
        //! });
        //! \endcode
        //!
        template <typename Callback>
        void ParallelForEachIndex(Callback func) const;

        //! Returns the linear index of the given 3D coordinate (pt.x, pt.y, pt.z)
        size_t Index(const Point3UI& pt) const;

        //! Returns the linear index of the given 3D coordinate (i,j,k)
        size_t Index(size_t i, size_t j, size_t k) const;

        //! Returns the refernce to the \p i-th element.
        T& operator[](size_t i);

        //! Returns the const refernece to the \p i-th element.
        const T& operator[](size_t i) const;

        //! Returns the reference to the element at (pt.x, pt.y, pt.z).
        T& operator()(const Point3UI& pt);

        //! Returns the const reference to the element at (pt.x, pt.y, pt.z).
        const T& operator()(const Point3UI& pt) const;

        //! Returns the reference to the element at (i, j, k).
        T& operator()(size_t i, size_t j, size_t k);

        //! Returns the const reference to the element at (i, j, k).
        const T& operator()(size_t i, size_t j, size_t k) const;

        //! Copies given array accessor \p other.
        ArrayAccessor& operator=(const ArrayAccessor& other);

        //! Casts type to ConstArrayAccessor.
        operator ConstArrayAccessor<T, 3>() const;

    private:
        Size3 _size;
        T* _data;
    };

    //! Type alias for 3D array accessor.
    template<typename T> using ArrayAccessor3 = ArrayAccessor<T,3>;


    //!
    //! \brief 3D read-only array accessor class.
    //!
    //! This class represents 3D read-only array accessor. It provides array-like
    //! data read/write function, but does not handle memory management.
    //!
    //! \see Array<T,3>
    //!

    template<typename T>
    class ConstArrayAccessor<T,3>
    {
    public:
        //! Constructs an empty 3D read-only array accessor.
        ConstArrayAccessor();

        //! Constructs a read-only array accessor that wraps given array.
        ConstArrayAccessor(const Size3& size, const T* const data);

        //! Constructs a read-only array accessor that wraps given array.
        ConstArrayAccessor(size_t width, size_t height, size_t depth, const T* const data);

        //! Constructs a read-only array accessor from read/write accessor.
        explicit ConstArrayAccessor(const ArrayAccessor<T,3>& other);

        //! Copy constructor.
        ConstArrayAccessor(const ConstArrayAccessor& other);

        //! Returns the reference to the \p i-th element.
        const T& At(size_t i) const;

        //! Returns the const reference to the element at (pt.x, pt.y, pt.z).
        const T& At(const Point3UI& pt) const;

        //! Returns the const reference to the element at (i,j, k).
        const T& At(size_t i, size_t j, size_t k) const;

        //! Returns the begin iterator of the array.
        const T* const begin() const;

        //! Returns the end iterator of the array.
        const T* const end() const;

        //! Returns the size of the array.
        Size3 Size() const;

        //! Returns the width of the array.
        size_t Width() const;

        //! Returns the height of the array.
        size_t Height() const;

        //! Returns the depth of the array.
        size_t Depth() const;

        //! Returns the raw pointer to the array data.
        const T* const Data() const;

        //! \brief Iterates the array and invoke given \p func for each index.
        //!
        //! This function iterates the array elements and invoke the callback
        //! function \p func. The callback function takes array's element as its
        //! input. The order of execution will be the same as the nested for-loop
        //! below:
        //!
        //! \code{.cpp}
        //! int data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}
        //! ConstArrayAccessor<int, 3> acc(2, 3, 2, data);
        //! for (size_t k = 0; k < acc.depth(); ++k) {
        //!     for (size_t j = 0; j < acc.height(); ++j) {
        //!         for (size_t i = 0; i < acc.width(); ++i) {
        //!             func(i, j, k);
        //!         }
        //!     }
        //! }
        //! \endcode
        //!
        //! Below is the sample usage:
        //!
        //! \code{.cpp}
        //! int data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}
        //! ConstArrayAccessor<int, 3> acc(2, 3, 2, data);
        //! acc.ForEach([](int elem) {
        //!     printf("%d\n", elem);
        //! });
        //! \endcode
        //!
        template <typename Callback>
        void ForEach(Callback func) const;

        //! \brief Iterates the array and invoke given \p func for each index.
        //!
        //! This function iterates the array elements and invoke the callback
        //! function \p func. The callback function takes three parameters which are
        //! the (i, j, j) indices of the array. The order of execution will be the
        //! same as the nested for-loop below:
        //!
        //! \code{.cpp}
        //! int data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}
        //! ConstArrayAccessor<int, 3> acc(2, 3, 2, data);
        //! for (size_t k = 0; k < acc.depth(); ++k) {
        //!     for (size_t j = 0; j < acc.height(); ++j) {
        //!         for (size_t i = 0; i < acc.width(); ++i) {
        //!             func(i, j, k);
        //!         }
        //!     }
        //! }
        //! \endcode
        //!
        //! Below is the sample usage:
        //!
        //! \code{.cpp}
        //! int data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}
        //! ConstArrayAccessor<int, 3> acc(2, 3, 2, data);
        //! acc.ForEachIndex([&](size_t i, size_t j, size_t k) {
        //!     acc(i, j, k) = 4.f * i + 7.f * j + 3.f * k + 1.5f;
        //! });
        //! \endcode
        template <typename Callback>
        void ForEachIndex(Callback func) const;


        //! \brief Iterates the array and invoke given \p func for each index in
        //!     parallel using multi-threading.
        //!
        //! This function iterates the array elements and invoke the callback
        //! function \p func in parallel using multi-threading. The callback
        //! function takes two parameters which are the (i, j, k) indices of the
        //! array. The order of execution will be non-deterministic since it runs in
        //! parallel. Below is the sample usage:
        //!
        //! \code{.cpp}
        //! int data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}
        //! ConstArrayAccessor<int, 3> acc(2, 3, 2, data);
        //! acc.ParallelForEachIndex([&](size_t i, size_t j, size_t k) {
        //!     acc(i, j, k) *= 2;
        //! });
        //! \endcode
        //!
        template <typename Callback>
        void ParallelForEachIndex(Callback func) const;


        //! Returns the linear index of the given 3D coordinate (pt.x, pt.y, pt.z)
        size_t Index(const Point3UI& pt) const;

        //! Returns the linear index of the given 3D coordinate (i, j, k)
        size_t Index(size_t i, size_t j, size_t k) const;

        //! Returns the const reference to the \p i-th element.
        const T& operator[](size_t i) const;

        //! Returns the const reference to the element at (pt.x, pt.y, pt.z)
        const T& operator()(const Point3UI& pt) const;

        //! Returns the const reference to the element at (i,j,k)
        const T& operator()(size_t i, size_t j, size_t k) const;
    private:
        Size3 _size;
        const T* _data;
    };

    //Type alias for 3D const array accessor.
    template<typename T> using ConstArrayAccessor3 = ConstArrayAccessor<T,3>;

    template<typename T>
    ArrayAccessor<T,3>::ArrayAccessor() : _data(nullptr)
    {}

    template<typename T>
    ArrayAccessor<T,3>::ArrayAccessor(const Size3& size, T* const data)
    {
        Reset(size, data);
    }

    template<typename T>
    ArrayAccessor<T,3>::ArrayAccessor(size_t width, size_t height, size_t depth, T* const data)
    {
        Reset(width, height, depth, data);
    }

    template<typename T>
    ArrayAccessor<T,3>::ArrayAccessor(const ArrayAccessor& other)
    {
        Set(other);
    }

    template<typename T>
    void ArrayAccessor<T,3>::Set(const ArrayAccessor& other)
    {
        Reset(other._size, other._data);
    }

    template<typename T>
    void ArrayAccessor<T,3>::Reset(const Size3& size, T* const data)
    {
        _size = size;
        _data = data;
    }

    template<typename T>
    void ArrayAccessor<T,3>::Reset(size_t width, size_t height, size_t depth, T* const data)
    {
        Reset(Size3(width, height, depth), data);
    }

    template<typename T>
    T& ArrayAccessor<T,3>::At(size_t i)
    {
        JET_ASSERT(i < _size.x * _size.y * _size.z);
        return _data[i];
    }

    template<typename T>
    const T& ArrayAccessor<T,3>::At(size_t i) const
    {
        JET_ASSERT(i < _size.x * _size.y * _size.z);
        return _data[i];
    }

    template<typename T>
    T& ArrayAccessor<T,3>::At(const Point3UI& pt)
    {
        return At(pt.x, pt.y, pt.z);
    }

    template<typename T>
    const T& ArrayAccessor<T,3>::At(const Point3UI& pt) const
    {
        return At(pt.x, pt.y, pt.z);
    }

    template<typename T>
    T& ArrayAccessor<T,3>::At(size_t i, size_t j, size_t k)
    {
        JET_ASSERT(i < _size.x && j<_size.y && k<_size.z);
        return _data[i + _size.x * (j + _size.y * k)];
    }

    template<typename T>
    const T& ArrayAccessor<T,3>::At(size_t i, size_t j, size_t k) const
    {
        JET_ASSERT(i < _size.x && j<_size.y && k<_size.k);
        return _data[i + _size.x * (j + _size.y * k) ];
    }

    template<typename T>
    T* const ArrayAccessor<T,3>::begin() const
    {
        return _data;
    }

    template<typename T>
    T* const ArrayAccessor<T,3>::end() const
    {
        return _data + _size.x * _size.y * _size.z;
    }

    template<typename T>
    T* ArrayAccessor<T,3>::begin()
    {
        return _data;
    }

    template<typename T>
    T* ArrayAccessor<T,3>::end()
    {
        return _data + _size.x * _size.y * _size.z;
    }

    template<typename T>
    Size3 ArrayAccessor<T,3>::Size() const
    {
        return _size;
    }

    template<typename T>
    size_t ArrayAccessor<T,3>::Width() const
    {
        return _size.x;
    }

    template<typename T>
    size_t ArrayAccessor<T,3>::Height() const
    {
        return _size.y;
    }

    template<typename T>
    size_t ArrayAccessor<T,3>::Depth() const
    {
        return _size.z;
    }

    template<typename T>
    T* const ArrayAccessor<T,3>::Data() const
    {
        return _data;
    }

    template<typename T>
    void ArrayAccessor<T,3>::Swap(ArrayAccessor& other)
    {
        std::swap(other._data, _data);
        std::swap(other._size, _size);
    }

    template<typename T>
    template<typename Callback>
    void ArrayAccessor<T,3>::ForEach(Callback func) const
    {
        for(size_t k = 0; k<_size.z; ++k)
        {
            for (size_t j = 0; j<_size.y; ++j)
            {
                for (size_t i = 0; i<_size.x; ++i)
                {
                    func(At(i,j,k));
                }
            }
        }
    }

    template<typename T>
    template<typename Callback>
    void ArrayAccessor<T,3>::ForEachIndex(Callback func) const
    {
        for(size_t k = 0; k<_size.z; ++k)
        {
            for (size_t j = 0; j<_size.y; ++j)
            {
                for (size_t i = 0; i<_size.x; ++i)
                {
                    func(i,j,k);
                }
            }
        }
    }


    template <typename T>
    template <typename Callback>
    void ArrayAccessor<T, 3>::ParallelForEach(Callback func) {
        ParallelFor(kZeroSize, _size.x, kZeroSize, _size.y, kZeroSize, _size.z,
            [&](size_t i, size_t j, size_t k) {
                func(At(i, j, k));
            });
    }

    template <typename T>
    template <typename Callback>
    void ArrayAccessor<T, 3>::ParallelForEachIndex(Callback func) const {
        ParallelFor(
            kZeroSize, _size.x, kZeroSize, _size.y, kZeroSize, _size.z, func);
    }


    template<typename T>
    size_t ArrayAccessor<T,3>::Index(const Point3UI& pt) const
    {
        JET_ASSERT(pt.x < _size.x && pt.y < _size.y && pt.z < _size.z);
        return pt.x + _size.x * (pt.y + _size.y * pt.z);
    }

    template<typename T>
    size_t ArrayAccessor<T,3>::Index(size_t i, size_t j, size_t k) const
    {
        JET_ASSERT(i < _size.x && j < _size.y && k < _size.z);
        return i + _size.x * (j * _size.y * k);
    }

    template<typename T>
    T& ArrayAccessor<T,3>::operator[](size_t i)
    {
        return _data[i];
    }

    template<typename T>
    const T& ArrayAccessor<T,3>::operator[](size_t i) const
    {
        return _data[i];
    }

    template<typename T>
    T& ArrayAccessor<T,3>::operator()(const Point3UI &pt)
    {
        JET_ASSERT(pt.x < _size.x && pt.y < _size.y && pt.z < _size.z);
        return _data[pt.x + _size.x * (pt.y + _size.y * pt.z)];
    }

    template<typename T>
    const T& ArrayAccessor<T,3>::operator()(const Point3UI& pt) const
    {
        JET_ASSERT(pt.x < _size.x && pt.y < _size.y && pt.z < _size.z);
        return _data[pt.x + _size.x * (pt.y + _size.y * pt.z)];
    }

    template<typename T>
    T& ArrayAccessor<T,3>::operator()(size_t i, size_t j, size_t k)
    {
        JET_ASSERT(i< _size.x && j < _size.y);
        return _data[i + _size.x * (j + _size.y * k)];
    }

    template<typename T>
    const T& ArrayAccessor<T,3>::operator()(size_t i, size_t j, size_t k) const
    {
        JET_ASSERT(i< _size.x && j < _size.y && k < _size.z);
        return _data[i + _size.x * (j + _size.y * k)];
    }

    template<typename T>
    ArrayAccessor<T,3>& ArrayAccessor<T,3>::operator=(const ArrayAccessor& other)
    {
        Set(other);
        return *this;
    }

    template<typename T>
    ArrayAccessor<T,3>::operator ConstArrayAccessor<T,3>() const
    {
        return ConstArrayAccessor<T,3>(*this);
    }

    template<typename T>
    ConstArrayAccessor<T,3>::ConstArrayAccessor() : _data(nullptr)
    {

    }

    template<typename T>
    ConstArrayAccessor<T,3>::ConstArrayAccessor( const Size3& size, const T* const data)
    {
        _size = size;
        _data = data;
    }

    template<typename T>
    ConstArrayAccessor<T,3>::ConstArrayAccessor(size_t width, size_t height, size_t depth, const T* const data)
    {
        _size = Size3(width, height, depth);
        _data = data;
    }
    
    template<typename T>
    ConstArrayAccessor<T,3>::ConstArrayAccessor(const ArrayAccessor<T,3>& other)
    {
        _size = other.Size();
        _data = other.Data();
    }

    template<typename T>
    ConstArrayAccessor<T,3>::ConstArrayAccessor(const ConstArrayAccessor& other)
    {
        _size = other._size;
        _data = other._data;
    }

    template<typename T>
    const T& ConstArrayAccessor<T,3>::At(size_t i) const
    {
        JET_ASSERT(i < _size.x * _size.y * _size.z);
        return _data[i];
    }

    template<typename T>
    const T& ConstArrayAccessor<T,3>::At(const Point3UI& pt) const
    {
        return At(pt.x, pt.y, pt.z);
    }

    template<typename T>
    const T& ConstArrayAccessor<T,3>::At(size_t i, size_t j, size_t k) const
    {
        JET_ASSERT(i < _size.x && j < _size.y && k < _size.z);
        return _data[i + _size.x * (j + _size.y * k)];
    }

    template<typename T>
    const T* const ConstArrayAccessor<T,3>::begin() const
    {
        return _data;
    }

    template<typename T>
    const T* const ConstArrayAccessor<T,3>::end() const
    {
        return _data + _size.x * _size.y * _size.z;
    }

    template<typename T>
    Size3 ConstArrayAccessor<T,3>::Size() const
    {
        return _size;
    }

    template<typename T>
    size_t ConstArrayAccessor<T,3>::Width() const
    {
        return _size.x;
    }

    template<typename T>
    size_t ConstArrayAccessor<T,3>::Height() const
    {
        return _size.y;
    }

    template<typename T>
    size_t ConstArrayAccessor<T,3>::Depth() const
    {
        return _size.z;
    }

    template<typename T>
    const T* const ConstArrayAccessor<T,3>::Data() const
    {
        return _data;
    }

    template <typename T>
    template <typename Callback>
    void ConstArrayAccessor<T, 3>::ForEach(Callback func) const {
        for(size_t k=0; k<_size.z; ++k){
            for (size_t j = 0; j < _size.y; ++j) {
                for (size_t i = 0; i < _size.x; ++i) {
                    func(At(i, j, k));
                }
            }
        }
    }

    template <typename T>
    template <typename Callback>
    void ConstArrayAccessor<T, 3>::ForEachIndex(Callback func) const {
        for (size_t k = 0; k<_size.z; ++k)
        {
            for (size_t j = 0; j < _size.y; ++j) {
                for (size_t i = 0; i < _size.x; ++i) {
                    func(i, j, k);
                }
            }
        }
    }

    template <typename T>
    template <typename Callback>
    void ConstArrayAccessor<T, 3>::ParallelForEachIndex(Callback func) const {
        ParallelFor(
            kZeroSize, _size.x, kZeroSize, _size.y, kZeroSize, _size.z, func);
    }


    template <typename T>
    size_t ConstArrayAccessor<T, 3>::Index(const Point3UI& pt) const {
        JET_ASSERT(pt.x < _size.x && pt.y < _size.y && pt.z < _size.z);
        return pt.x + _size.x * (pt.y + _size.y * pt.z);
    }

    template <typename T>
    size_t ConstArrayAccessor<T, 3>::Index(size_t i, size_t j, size_t k) const {
        JET_ASSERT(i < _size.x && j < _size.y && k < _size.z);
        return i + _size.x * (j + _size.y * k);
    }

    template <typename T>
    const T& ConstArrayAccessor<T, 3>::operator[](size_t i) const {
        return _data[i];
    }

    template <typename T>
    const T& ConstArrayAccessor<T, 3>::operator()(const Point3UI &pt) const {
        JET_ASSERT(pt.x < _size.x && pt.y < _size.y && pt.z < _size.z);
        return _data[pt.x + _size.x * (pt.y * _size.y + pt.z)];
    }

    template <typename T>
    const T& ConstArrayAccessor<T, 3>::operator()(size_t i, size_t j, size_t k) const {
        JET_ASSERT(i < _size.x && j < _size.y && k < _size.z);
        return _data[i + _size.x * (j + _size.y * k)];
    }

}