#pragma once

#include <Arrays/array_accessor.h>
#include <Points/point2.h>
#include <Size/size2.h>
#include <utility>

#include <macros.h>


namespace jet
{
    //!
    //! \brief 2-D Array Accessor Class.
    //!
    //! This class implements 2-D array accessor, which provides
    //! array-like data read/write functions. This class does not
    //! handle memory management.
    //! 
    //! \see Array<T,2>
    //!
    //! \tparam T - Array Value Type
    //!

    template<typename T>
    class ArrayAccessor<T,2> final
    {
    public:
        //! Constructs empty 2D array accessor.
        ArrayAccessor();

        //!
        //! \brief Constructs an array accessor that wraps given array.
        //! \param size Size of the 2D array
        //! \param data Raw array pointer.
        ArrayAccessor( const Size2& size, T* const data);

        ArrayAccessor(size_t width, size_t height, T* const data);

        //! Copy Constructor
        ArrayAccessor(const ArrayAccessor& other);

        //! Replaces the content with given \p other array accessor.
        void Set(const ArrayAccessor& other);

        //! Resets the array.
        void Reset(const Size2& size, T* const data);

        //! Resets the array.
        void Reset(size_t width, size_t height, T* const data);

        //! Returns the reference to the \p i-th element.
        T& At(size_t i);

        //! Returns the const reference to the \p i-th element.
        const T& At(size_t i) const;

        //! Returns the reference to the element at (pt.x, pt.y)
        T& At(const Point2UI& pt);

        //! Returns the const reference to the element at (pt.x, pt.y)
        const T& At(const Point2UI& pt) const;

        //! Returns the reference to the element at (i,j)
        T& At(size_t i, size_t j);

        //! Returns the const reference to the element at (i,j)
        const T& At(size_t i, size_t j) const;

        //! Returns the begin iterator of the array.
        T* const begin() const;

        //! Returns the end iterator of the array.
        T* const end() const;

        //! Returns the begin iterator of the array.
        T* begin();

        //! Returns the end iterator of the array.
        T* end();

        //! Returns the size of the array.
        Size2 Size() const;

        //! Returns the width of the array.
        size_t Width() const;

        //! Returns the height of the array.
        size_t Height() const;

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
        //! int data = {1, 2, 3, 4, 5, 6};
        //! ArrayAccessor<int, 2> acc(2, 3, data);
        //! for (size_t j = 0; j < acc.height(); ++j) {
        //!     for (size_t i = 0; i < acc.width(); ++i) {
        //!         func(acc(i, j));
        //!     }
        //! }
        //! \endcode
        //!
        //! Below is the sample usage:
        //!
        //! \code{.cpp}
        //! int data = {1, 2, 3, 4, 5, 6};
        //! ArrayAccessor<int, 2> acc(2, 3, data);
        //! acc.ForEach([](int elem) {
        //!     printf("%d\n", elem);
        //! });
        //! \endcode
        //!
        template<typename Callback>
        void ForEach(Callback func) const;

        //! \brief Iterates the array and invoke given \p func for each index.
        //!
        //! This function iterates the array elements and invoke the callback
        //! function \p func. The callback function takes two parameters which are
        //! the (i, j) indices of the array. The order of execution will be the same
        //! as the nested for-loop below:
        //!
        //! \code{.cpp}
        //! int data = {1, 2, 3, 4, 5, 6};
        //! ArrayAccessor<int, 2> acc(2, 3, data);
        //! for (size_t j = 0; j < acc.height(); ++j) {
        //!     for (size_t i = 0; i < acc.width(); ++i) {
        //!         func(i, j);
        //!     }
        //! }
        //! \endcode
        //!
        //! Below is the sample usage:
        //!
        //! \code{.cpp}
        //! int data = {1, 2, 3, 4, 5, 6};
        //! ArrayAccessor<int, 2> acc(2, 3, data);
        //! acc.ForEachIndex([&](size_t i, size_t j) {
        //!     acc(i, j) = 4.f * i + 7.f * j + 1.5f;
        //! });
        //! \endcode
        //!
        template <typename Callback>
        void ForEachIndex(Callback func) const;


        //TODO: Implement parallel methods

        //! Returns the linear index of the given 2D coordinate (pt.x, pt.y)
        size_t Index(const Point2UI& pt) const;

        //! Returns the linear index of the given 2D coordinate (i,j)
        size_t Index(size_t i, size_t j) const;

        //! Returns the refernce to the \p i-th element.
        T& operator[](size_t i);

        //! Returns the const refernece to the \p i-th element.
        const T& operator[](size_t i) const;

        //! Returns the reference to the element at (pt.x, pt.y).
        T& operator()(const Point2UI& pt);

        //! Returns the const reference to the element at (pt.x, pt.y).
        const T& operator()(const Point2UI& pt) const;

        //! Returns the reference to the element at (i, j).
        T& operator()(size_t i, size_t j);

        //! Returns the const reference to the element at (i, j).
        const T& operator()(size_t i, size_t j) const;

        //! Copies given array accessor \p other.
        ArrayAccessor& operator=(const ArrayAccessor& other);

        //! Casts type to ConstArrayAccessor.
        operator ConstArrayAccessor<T, 2>() const;

    private:
        Size2 _size;
        T* _data;
    };

    //! Type alias for 2D array accessor.
    template<typename T> using ArrayAccessor2 = ArrayAccessor<T,2>;


    //!
    //! \brief 2D read-only array accessor class.
    //!
    //! This class represents 2D read-only array accessor. It provides array-like
    //! data read/write function, but does not handle memory management.
    //!
    //! \see Array2<T,2>
    //!

    template<typename T>
    class ConstArrayAccessor<T,2>
    {
    public:
        //! Constructs an empty 2D read-only array accessor.
        ConstArrayAccessor();

        //! Constructs a read-only array accessor that wraps given array.
        ConstArrayAccessor(const Size2& size, const T* const data);

        //! Constructs a read-only array accessor that wraps given array.
        ConstArrayAccessor(size_t width, size_t height, const T* const data);

        //! Constructs a read-only array accessor from read/write accessor.
        explicit ConstArrayAccessor(const ArrayAccessor<T,2>& other);

        //! Copy constructor.
        ConstArrayAccessor(const ConstArrayAccessor& other);

        //! Returns the reference to the \p i-th element.
        const T& At(size_t i) const;

        //! Returns the const reference to the element at (pt.x, pt.y).
        const T& At(const Point2UI& pt) const;

        //! Returns the const reference to the element at (i,j).
        const T& At(size_t i, size_t j) const;

        //! Returns the begin iterator of the array.
        const T* const begin() const;

        //! Returns the end iterator of the array.
        const T* const end() const;

        //! Returns the size of the array.
        Size2 Size() const;

        //! Returns the width of the array.
        size_t Width() const;

        //! Returns the height of the array.
        size_t Height() const;

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
        //! int data = {1, 2, 3, 4, 5, 6};
        //! ConstArrayAccessor<int, 2> acc(2, 3, data);
        //! for (size_t j = 0; j < acc.height(); ++j) {
        //!     for (size_t i = 0; i < acc.width(); ++i) {
        //!         func(acc(i, j));
        //!     }
        //! }
        //! \endcode
        //!
        //! Below is the sample usage:
        //!
        //! \code{.cpp}
        //! int data = {1, 2, 3, 4, 5, 6};
        //! ConstArrayAccessor<int, 2> acc(2, 3, data);
        //! acc.ForEach([](int elem) {
        //!     printf("%d\n", elem);
        //! });
        //! \endcode
        //!
        template <typename Callback>
        void ForEach(Callback func) const;

        //!
        //! \brief Iterates the array and invoke given \p func for each index.
        //!
        //! This function iterates the array elements and invoke the callback
        //! function \p func. The callback function takes two parameters which are
        //! the (i, j) indices of the array. The order of execution will be the same
        //! as the nested for-loop below:
        //!
        //! \code{.cpp}
        //! int data = {1, 2, 3, 4, 5, 6};
        //! ConstArrayAccessor<int, 2> acc(2, 3, data);
        //! for (size_t j = 0; j < acc.height(); ++j) {
        //!     for (size_t i = 0; i < acc.width(); ++i) {
        //!         func(i, j);
        //!     }
        //! }
        //! \endcode
        //!
        //! Below is the sample usage:
        //!
        //! \code{.cpp}
        //! int data = {1, 2, 3, 4, 5, 6};
        //! ConstArrayAccessor<int, 2> acc(2, 3, data);
        //! acc.ForEachIndex([&](size_t i, size_t j) {
        //!     acc(i, j) = 4.f * i + 7.f * j + 1.5f;
        //! });
        //! \endcode
        //!
        template <typename Callback>
        void ForEachIndex(Callback func) const;


        //TODO: Implement PARALLEL foreach implementation.


        //! Returns the linear index of the given 2D coordinate (pt.x, pt.y)
        size_t Index(const Point2UI& pt) const;

        //! Returns the linear index of the given 2D coordinate (i, j)
        size_t Index(size_t i, size_t j) const;

        //! Returns the const reference to the \p i-th element.
        const T& operator[](size_t i) const;

        //! Returns the const reference to the element at (pt.x, pt.y)
        const T& operator()(const Point2UI& pt) const;

        //! Returns the const reference to the element at (i,j)
        const T& operator()(size_t i, size_t j) const;
    private:
        Size2 _size;
        const T* _data;
    };

    //Type alias for 2D const array accessor.
    template<typename T> using ConstArrayAccessor2 = ConstArrayAccessor<T,2>;

    template<typename T>
    ArrayAccessor<T,2>::ArrayAccessor() : _data(nullptr)
    {}

    template<typename T>
    ArrayAccessor<T,2>::ArrayAccessor(const Size2& size, T* const data)
    {
        Reset(size, data);
    }

    template<typename T>
    ArrayAccessor<T,2>::ArrayAccessor(size_t width, size_t height, T* const data)
    {
        Reset(width, height, data);
    }

    template<typename T>
    ArrayAccessor<T,2>::ArrayAccessor(const ArrayAccessor& other)
    {
        Set(other);
    }

    template<typename T>
    void ArrayAccessor<T,2>::Set(const ArrayAccessor& other)
    {
        Reset(other._size, other._data);
    }

    template<typename T>
    void ArrayAccessor<T,2>::Reset(const Size2& size, T* const data)
    {
        _size = size;
        _data = data;
    }

    template<typename T>
    void ArrayAccessor<T,2>::Reset(size_t width, size_t height, T* const data)
    {
        Reset(Size2(width, height), data);
    }

    template<typename T>
    T& ArrayAccessor<T,2>::At(size_t i)
    {
        JET_ASSERT(i < _size.x * _size.y);
        return _data[i];
    }

    template<typename T>
    const T& ArrayAccessor<T,2>::At(size_t i) const
    {
        JET_ASSERT(i < _size.x * _size.y);
        return _data[i];
    }

    template<typename T>
    T& ArrayAccessor<T,2>::At(const Point2UI& pt)
    {
        return At(pt.x, pt.y);
    }

    template<typename T>
    const T& ArrayAccessor<T,2>::At(const Point2UI& pt) const
    {
        return At(pt.x, pt.y);
    }

    template<typename T>
    T& ArrayAccessor<T,2>::At(size_t i, size_t j)
    {
        JET_ASSERT(i < _size.x && j<_size.y);
        return _data[i + _size.x * j];
    }

    template<typename T>
    const T& ArrayAccessor<T,2>::At(size_t i, size_t j) const
    {
        JET_ASSERT(i < _size.x && j<_size.y);
        return _data[i + _size.x * j];
    }

    template<typename T>
    T* const ArrayAccessor<T,2>::begin() const
    {
        return _data;
    }

    template<typename T>
    T* const ArrayAccessor<T,2>::end() const
    {
        return _data + _size.x * _size.y;
    }

    template<typename T>
    T* ArrayAccessor<T,2>::begin()
    {
        return _data;
    }

    template<typename T>
    T* ArrayAccessor<T,2>::end()
    {
        return _data + _size.x * _size.y;
    }

    template<typename T>
    Size2 ArrayAccessor<T,2>::Size() const
    {
        returns _size;
    }

    template<typename T>
    size_t ArrayAccessor<T,2>::Width() const
    {
        return _size.x;
    }

    template<typename T>
    size_t ArrayAccessor<T,2>::Height() const
    {
        return _size.y;
    }

    template<typename T>
    T* const ArrayAccessor<T,2>::Data() const
    {
        return _data;
    }

    template<typename T>
    void ArrayAccessor<T,2>::Swap(ArrayAccessor& other)
    {
        std::swap(other._data, _data);
        std::swap(other._size, _size);
    }

    template<typename T>
    template<typename Callback>
    void ArrayAccessor<T,2>::ForEach(Callback func) const
    {
        for (size_t j = 0; j<_size.y; ++j)
        {
            for (size_t i = 0; i<_size.x; ++i)
            {
                func(At(i,j));
            }
        }
    }

    template<typename T>
    template<typename Callback>
    void ArrayAccessor<T,2>::ForEachIndex(Callback func) const
    {
        for (size_t j = 0; j<_size.y; ++j)
        {
            for (size_t i = 0; i<_size.x; ++i)
            {
                func(i,j);
            }
        }
    }








    template<typename T>
    size_t ArrayAccessor<T,2>::Index(const Point2UI& pt) const
    {
        JET_ASSERT(pt.x < _size.x && pt.y < _size.y);
        return pt.x + _size.x * pt.y;
    }

    template<typename T>
    size_t ArrayAccessor<T,2>::Index(size_t i, size_t j) const
    {
        JET_ASSERT(i < _size.x && j < _size.y);
        return i + _size.x * j;
    }

    template<typename T>
    T& ArrayAccessor<T,2>::operator[](size_t i)
    {
        return _data[i];
    }

    template<typename T>
    const T& ArrayAccessor<T,2>::operator[](size_t i) const
    {
        return _data[i];
    }

    template<typename T>
    T& ArrayAccessor<T,2>::operator()(const Point2UI &pt)
    {
        JET_ASSERT(pt.x < _size.x && pt.y < _size.y);
        return _data[pt.x + _size.x * pt.y];
    }

    template<typename T>
    const T& ArrayAccessor<T,2>::operator()(const Point2UI& pt) const
    {
        JET_ASSERT(pt.x < _size.x && pt.y < _size.y);
        return _data[pt.x + _size.x * pt.y];
    }

    template<typename T>
    T& ArrayAccessor<T,2>::operator()(size_t i, size_t j)
    {
        JET_ASSERT(i< _size.x && j < _size.y);
        return _data[i + _size.x * j];
    }

    template<typename T>
    const T& ArrayAccessor<T,2>::operator()(size_t i, size_t j) const
    {
        JET_ASSERT(i< _size.x && j < _size.y);
        return _data[i + _size.x * j];
    }

    template<typename T>
    ArrayAccessor<T,2>& ArrayAccessor<T,2>::operator=(const ArrayAccessor& other)
    {
        Set(other);
        return *this;
    }

    template<typename T>
    ArrayAccessor<T,2>::operator ConstArrayAccessor<T,2>() const
    {
        return ConstArrayAccessor<T,2>(*this);
    }

    template<typename T>
    ConstArrayAccessor<T,2>::ConstArrayAccessor() : _data(nullptr)
    {

    }

    template<typename T>
    ConstArrayAccessor<T,2>::ConstArrayAccessor( const Size2& size, const T* const data)
    {
        _size = size;
        _data = data;
    }

    template<typename T>
    ConstArrayAccessor<T,2>::ConstArrayAccessor(size_t width, size_t height, const T* const data)
    {
        _size = Size2(width, height);
        _data = data;
    }
    
    template<typename T>
    ConstArrayAccessor<T,2>::ConstArrayAccessor(const ArrayAccessor<T,2>& other)
    {
        _size = other.Size();
        _data = other.Data();
    }

    template<typename T>
    ConstArrayAccessor<T,2>::ConstArrayAccessor(const ConstArrayAccessor& other)
    {
        _size = other._size;
        _data = other._data;
    }

    template<typename T>
    const T& ConstArrayAccessor<T,2>::At(size_t i) const
    {
        JET_ASSERT(i < _size.x * _size.y);
        return _data[i];
    }

    template<typename T>
    const T& ConstArrayAccessor<T,2>::At(const Point2UI& pt) const
    {
        return At(pt.x, pt.y);
    }

    template<typename T>
    const T& ConstArrayAccessor<T,2>::At(size_t i, size_t j) const
    {
        JET_ASSERT(i < _size.x && j < _size.y);
        return _data[i + _size.x * j];
    }

    template<typename T>
    const T* const ConstArrayAccessor<T,2>::begin() const
    {
        return _data;
    }

    template<typename T>
    const T* const ConstArrayAccessor<T,2>::end() const
    {
        return _data + _size.x * _size.y;
    }

    template<typename T>
    Size2 ConstArrayAccessor<T,2>::Size() const
    {
        return _size;
    }

    template<typename T>
    size_t ConstArrayAccessor<T,2>::Width() const
    {
        return _size.x;
    }

    template<typename T>
    size_t ConstArrayAccessor<T,2>::Height() const
    {
        return _size.y;
    }

    template<typename T>
    const T* const ConstArrayAccessor<T,2>::Data() const
    {
        return _data;
    }

    template <typename T>
    template <typename Callback>
    void ConstArrayAccessor<T, 2>::ForEach(Callback func) const {
        for (size_t j = 0; j < _size.y; ++j) {
            for (size_t i = 0; i < _size.x; ++i) {
                func(at(i, j));
            }
        }
    }

    template <typename T>
    template <typename Callback>
    void ConstArrayAccessor<T, 2>::ForEachIndex(Callback func) const {
        for (size_t j = 0; j < _size.y; ++j) {
            for (size_t i = 0; i < _size.x; ++i) {
                func(i, j);
            }
        }
    }




    template <typename T>
    size_t ConstArrayAccessor<T, 2>::Index(const Point2UI& pt) const {
        JET_ASSERT(pt.x < _size.x && pt.y < _size.y);
        return pt.x + _size.x * pt.y;
    }

    template <typename T>
    size_t ConstArrayAccessor<T, 2>::Index(size_t i, size_t j) const {
        JET_ASSERT(i < _size.x && j < _size.y);
        return i + _size.x * j;
    }

    template <typename T>
    const T& ConstArrayAccessor<T, 2>::operator[](size_t i) const {
        return _data[i];
    }

    template <typename T>
    const T& ConstArrayAccessor<T, 2>::operator()(const Point2UI &pt) const {
        JET_ASSERT(pt.x < _size.x && pt.y < _size.y);
        return _data[pt.x + _size.x * pt.y];
    }

    template <typename T>
    const T& ConstArrayAccessor<T, 2>::operator()(size_t i, size_t j) const {
        JET_ASSERT(i < _size.x && j < _size.y);
        return _data[i + _size.x * j];
    }

}