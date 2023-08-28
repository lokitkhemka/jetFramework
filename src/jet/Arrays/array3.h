#pragma once

#include <Arrays/array.h>
#include <Arrays/array3_accessor.h>
#include <fstream>
#include <functional>
#include <iostream>
#include <vector>

#include <macros.h>

namespace jet
{
    //! \brief 3-D array class.
    //!
    //! This class represents 3-D array data structure. Internally, the 3-D data is
    //! mapped to a linear array such that (i, j, k) element is actually stored at
    //! (i + (width * (j + height * k)))th element of the linear array. This mapping means
    //! iterating i first and j and k next will give better performance such as:
    //!
    //! \code{.cpp}
    //! Array<int, 3> array;
    //! for(size_t k = 0; k<array.Depth(); ++k){
    //!     for (size_t j = 0; j < array.height(); ++j) {
    //!         for (size_t i = 0; i < array.width(); ++i) {
    //!             // Read or write array(i, j)
    //!         }
    //!     }
    //! }
    //! \endcode
    //!
    //! \tparam T - Type to store in the array.
    template <typename T>
    class Array<T, 3> final
    {
    public:
        typedef std::vector<T> ContainerType;

        //! Constructs zero-sized 3D array
        Array();

        //! Constructs 3D array with given \p size and fill it with \p initVal.
        explicit Array(const Size3& size, const T& initVal=T());

        //! Constructs 3D array with size \p width times \p height times \p depth  and fill it with \p initVal.
        Array(size_t width, size_t height, size_t depth, const T& initVal = T());

        //! \brief Constructs 3-D array with given initializer list \p list.
        //!
        //! This constructor will build 3-D array with given initializer list \p list
        //! such as
        //!
        //! \code{.cpp}
        //! Array<int, 3> arr = {
        //!     {{ 1.f,  2.f,  3.f,  4.f},
        //!      { 5.f,  6.f,  7.f,  8.f},
        //!      { 9.f, 10.f, 11.f, 12.f}},
        //!     {{13.f, 14.f, 15.f, 16.f},
        //!      {17.f, 18.f, 19.f, 20.f},
        //!      {21.f, 22.f, 23.f, 24.f}}};
        //! \endcode
        //!
        //! Note the initializer also has 3-D structure. The code above will
        //! construct 4 x 3 x 2 array.
        //!
        //! \param list Initializer list that should be copy to the new array.
        //!
        Array(
            const std::initializer_list<
                std::initializer_list<std::initializer_list<T>>>& list);

        //! Copy constructor
        Array(const Array& other);

        //! Sets entire array with given \p value.
        void Set(const T& value);

        //! Copies the given array \p other to this array.
        void Set(const Array& other);

        //! Copies given initializer list \p list to this array.
        //!
        //! This function copies given initializer list \p list to the array such as
        //!
        //! \code{.cpp}
        //! Array<int, 3> arr;
        //! arr = {
        //!     {{ 1.f,  2.f,  3.f,  4.f},
        //!      { 5.f,  6.f,  7.f,  8.f},
        //!      { 9.f, 10.f, 11.f, 12.f}},
        //!     {{13.f, 14.f, 15.f, 16.f},
        //!      {17.f, 18.f, 19.f, 20.f},
        //!      {21.f, 22.f, 23.f, 24.f}}};
        //! \endcode
        //!
        //! Note the initializer also has 3-D structure. The code above will
        //! construct 4 x 3 x 2 array.
        //!
        //! \param list Initializer list that should be copy to the new array.
        //!
        void Set(
            const std::initializer_list<
                std::initializer_list<std::initializer_list<T>>>& list);

        //!Clears the array and resizes to zero.
        void Clear();

        //! Resizes the array with \p size and fill the new elements with \p initVal.
        void Resize(const Size3& size, const T& initVal=T());

        //! Resizes the array with size \p width x \p height x \p depth and fill the new
        //! element with \p initVal.
        void Resize(size_t width, size_t height, size_t depth, const T& initVal = T());

        //! Returns the reference to the i-th element, where i = x + (width*y)
        T& At(size_t i);

        //! Returns the const reference to the i-th element, where i = x+(width*y)
        const T& At(size_t i) const;

        //! Returns the reference to the element at (pt.x, pt.y, pt.z)
        T& At(const Point3UI& pt);

        //! Returns the const reference to the element at (pt.x, pt.y, pt.z)
        const T& At(const Point3UI& pt) const;

        //! Returns the reference to the element at (i, j, k).
        T& At(size_t i, size_t j, size_t k);

        //! Returns the const reference to the element at (i, j, k).
        const T& At(size_t i, size_t j, size_t k) const;

        //! Returns the size of the array.
        Size3 Size() const;

        //! Returns the Width of the array.
        size_t Width() const;

        //! Returns the Height of the array.
        size_t Height() const;

        //! Returns the depth of the array.
        size_t Depth() const;

        //! Returns the raw pointer to the data.
        T* Data();

        //! Returns the const raw pointer to the array data.
        const T* const Data() const;

        //! Returns the begin iterator of the array.
        typename ContainerType::iterator begin();

        //! Returns the begin const iterator of the array.
        typename ContainerType::const_iterator begin() const;

        //! Returns the end iterator of the array.
        typename ContainerType::iterator end();

        //! Returns the end const iterator of the array.
        typename ContainerType::const_iterator end() const;

        //! Returns the array accessor
        ArrayAccessor3<T> Accessor();

        //! Returns the const array accessor.
        ConstArrayAccessor3<T> ConstAccessor() const;

        //! Swaps the content of the array with  \p other array.
        void Swap(Array& other);

        //! \brief Iterates the array and invoke given \p func for each index.
        //!
        //! This function iterates the array elements and invoke the callback
        //! function \p func. The callback function takes array's element as its
        //! input. The order of execution will be the same as the nested for-loop
        //! below:
        //!
        //! \code{.cpp}
        //! Array<int, 3> array;
        //! for (size_t k = 0; k < array.depth(); ++k) {
        //!     for (size_t j = 0; j < array.height(); ++j) {
        //!         for (size_t i = 0; i < array.width(); ++i) {
        //!             func(i, j, k);
        //!         }
        //!     }
        //! }
        //! \endcode
        //!
        //! Below is the sample usage:
        //!
        //! \code{.cpp}
        //! Array<int, 3> array(100, 200, 150, 4);
        //! array.ForEach([](int elem) {
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
        //! Array<int, 3> array;
        //! for (size_t k = 0; k < array.depth(); ++k) {
        //!     for (size_t j = 0; j < array.height(); ++j) {
        //!         for (size_t i = 0; i < array.width(); ++i) {
        //!             func(i, j, k);
        //!         }
        //!     }
        //! }
        //! \endcode
        //!
        //! Below is the sample usage:
        //!
        //! \code{.cpp}
        //! Array<int, 3> array(10, 4);
        //! array.ForEachIndex([&](size_t i, size_t j, size_t k) {
        //!     array(i, j, k) = 4.f * i + 7.f * j + 3.f * k + 1.5f;
        //! });
        //! \endcode
        //!
        template <typename Callback>
        void ForEachIndex(Callback func) const;

        ///TODO: Implement Parallel foreach methods


        //! Returns the reference to the i-th element, where i = x + (width * (y + height * z))
        //! \see Array<T,3>::At
        T& operator[](size_t i);

        //! Returns the const reference to the i-th element, where i = x+(width * (y + height * z))
        //! \see Array<T,3>::At
        const T& operator[](size_t i) const;

        //! Returns the reference to the element at (i,j,k)
        T& operator()(size_t i, size_t j, size_t k);

        //! Returns the const reference to the element at (i,j,k)
        const T& operator()(size_t i, size_t j, size_t k) const;

        //! Returns the reference to the element at (pt.x, pt.y, pt.z)
        T& operator()(const Point3UI& pt);

        //! Returns the const reference to the element at (pt.x, pt.y, pt.z)
        const T& operator()(const Point3UI& pt) const;

        //! Sets the array with given \p value.
        Array& operator=(const T& other);

        //! Copies given array \p other to this array.
        Array& operator=(const Array& other);


        //! Copies given initializer list \p list to this array.
        //!
        //! This function copies given initializer list \p list to the array such as
        //!
        //! \code{.cpp}
        //! Array<int, 3> arr;
        //! arr = {
        //!     {{ 1.f,  2.f,  3.f,  4.f},
        //!      { 5.f,  6.f,  7.f,  8.f},
        //!      { 9.f, 10.f, 11.f, 12.f}},
        //!     {{13.f, 14.f, 15.f, 16.f},
        //!      {17.f, 18.f, 19.f, 20.f},
        //!      {21.f, 22.f, 23.f, 24.f}}};
        //! \endcode
        //!
        //! Note the initializer also has 3-D structure. The code above will
        //! construct 4 x 3 x 2 array.
        //!
        //! \param list Initializer list that should be copy to the new array.
        //!
        Array& operator=(
            const std::initializer_list<
                std::initializer_list<std::initializer_list<T>>>& list);


        //! Casts to array accessor.
        operator ArrayAccessor3<T>();

        //! Casts to const array accessor.
        operator ConstArrayAccessor3<T>() const;

    private:
        Size3 _size;
        std::vector<T> _data;
    };

    template<typename T> using Array3 = Array<T,3>;

    template<typename T>
    Array<T,3>::Array()
    {}

    template<typename T>
    Array<T,3>::Array(const Size3& size, const T& initVal)
    {
        Resize(size, initVal);
    }

    template<typename T>
    Array<T,3>::Array(size_t width, size_t height, size_t depth, const T& initVal)
    {
        Resize(width, height, depth, initVal);
    }

    template<typename T>
    Array<T,3>::Array(const std::initializer_list<std::initializer_list<std::initializer_list<T>>>& list)
    {
        Set(list);
    }

    template<typename T>
    Array<T,3>::Array(const Array& other)
    {
        Set(other);
    }

    template<typename T>
    void Array<T,3>::Set(const T& value)
    {
        for (auto& v : _data)
        {
            v = value;
        }
    }

    template<typename T>
    void Array<T,3>::Set(const Array& other)
    {
        _data.resize(other._data.size());
        std::copy(other._data.begin(), other._data.end(), _data.begin());
        _size = other._size;
    }

    template <typename T>
    void Array<T, 3>::Set(
        const std::initializer_list<
            std::initializer_list<std::initializer_list<T>>>& lst) {
        size_t depth = lst.size();
        auto pageIter = lst.begin();
        size_t height = (depth > 0) ? pageIter->size() : 0;
        auto rowIter = pageIter->begin();
        size_t width = (height > 0) ? rowIter->size() : 0;
        Resize(Size3(width, height, depth));
        for (size_t k = 0; k < depth; ++k) {
            rowIter = pageIter->begin();
            JET_ASSERT(height == pageIter->size());
            for (size_t j = 0; j < height; ++j) {
                auto colIter = rowIter->begin();
                JET_ASSERT(width == rowIter->size());
                for (size_t i = 0; i < width; ++i) {
                    (*this)(i, j, k) = *colIter;
                    ++colIter;
                }
                ++rowIter;
            }
            ++pageIter;
        }
    }

    template<typename T>
    void Array<T,3>::Clear()
    {
        _data.clear();
        _size = Size3(0,0,0);
    }

    template <typename T>
    void Array<T, 3>::Resize(const Size3& size, const T& initVal) {
        Array grid;
        grid._data.resize(size.x * size.y * size.z, initVal);
        grid._size = size;
        size_t iMin = std::min(size.x, _size.x);
        size_t jMin = std::min(size.y, _size.y);
        size_t kMin = std::min(size.z, _size.z);
        for (size_t k = 0; k < kMin; ++k) {
            for (size_t j = 0; j < jMin; ++j) {
                for (size_t i = 0; i < iMin; ++i) {
                    grid(i, j, k) = At(i, j, k);
                }
            }
        }

        Swap(grid);
    }

    template<typename T>
    void Array<T,3>::Resize(size_t width, size_t height, size_t depth, const T& initVal)
    {
        Resize(Size3(width,height,depth), initVal);
    }

    template<typename T>
    T& Array<T,3>::At(size_t i)
    {
        JET_ASSERT(i< _size.x * _size.y * _size.z);
        return _data[i];
    }

    template<typename T>
    const T& Array<T,3>::At(size_t i) const
    {
        JET_ASSERT(i < _size.x * _size.y * _size.z);
        return _data[i];
    }

    template<typename T>
    T& Array<T,3>::At(const Point3UI& pt)
    {
        return At(pt.x, pt.y, pt.z);
    }

    template<typename T>
    const T& Array<T,3>::At(const Point3UI& pt) const
    {
        return At(pt.x, pt.y, pt.z);
    }

    template<typename T>
    T& Array<T,3>::At(size_t i, size_t j, size_t k)
    {
        JET_ASSERT(i < _size.x && j < _size.y && k < _size.z);
        return _data[i + _size.x * ( j + _size.y * k)];
    }

    template<typename T>
    const T& Array<T,3>::At(size_t i, size_t j, size_t k) const
    {
        JET_ASSERT(i < _size.x && j < _size.y && k < _size.z);
        return _data[i + _size.x * (j + _size.y * k)];
    }

    template<typename T>
    Size3 Array<T,3>::Size() const
    {
        return _size;
    }

    template<typename T>
    size_t Array<T,3>::Width() const
    {
        return _size.x;
    }

    template<typename T>
    size_t Array<T,3>::Height() const
    {
        return _size.y;
    }

    template<typename T>
    size_t Array<T,3>::Depth() const
    {
        return _size.z;
    }

    template<typename T>
    T* Array<T,3>::Data()
    {
        return _data.data();
    }

    template<typename T>
    const T* const Array<T,3>::Data() const
    {
        return _data.data();
    }

    template<typename T>
    typename Array<T,3>::ContainerType::iterator Array<T,3>::begin()
    {
        return _data.begin();
    }

    template<typename T>
    typename Array<T,3>::ContainerType::const_iterator Array<T,3>::begin() const
    {
        return _data.begin();
    }

    template<typename T>
    typename Array<T,3>::ContainerType::iterator Array<T,3>::end()
    {
        return _data.end();
    }

    template<typename T>
    typename Array<T,3>::ContainerType::const_iterator Array<T,3>::end() const
    {
        return _data.end();
    }

    template<typename T>
    ArrayAccessor3<T> Array<T,3>::Accessor()
    {
        return ArrayAccessor3<T>(Size(), Data());
    }

    template<typename T>
    ConstArrayAccessor3<T> Array<T,3>::ConstAccessor() const
    {
        return ConstArrayAccessor3<T>(Size(), Data());
    }

    template<typename T>
    void Array<T,3>::Swap(Array& other)
    {
        std::swap(other._data, _data);
        std::swap(other._size, _size);
    }

    template <typename T>
    template <typename Callback>
    void Array<T, 3>::ForEach(Callback func) const {
        ConstAccessor().ForEach(func);
    }

    template <typename T>
    template <typename Callback>
    void Array<T, 3>::ForEachIndex(Callback func) const {
        ConstAccessor().ForEachIndex(func);
    }





    template <typename T>
    T& Array<T, 3>::operator[](size_t i) {
        return _data[i];
    }

    template <typename T>
    const T& Array<T, 3>::operator[](size_t i) const {
        return _data[i];
    }

    template <typename T>
    T& Array<T, 3>::operator()(size_t i, size_t j, size_t k) {
        JET_ASSERT(i < _size.x && j < _size.y && k < _size.z);
        return _data[i + _size.x * (j + _size.y * k)];
    }

    template <typename T>
    const T& Array<T, 3>::operator()(size_t i, size_t j, size_t k) const {
        JET_ASSERT(i < _size.x && j < _size.y && k < _size.z);
        return _data[i + _size.x * (j + _size.y * k)];
    }

    template <typename T>
    T& Array<T, 3>::operator()(const Point3UI &pt) {
        JET_ASSERT(pt.x < _size.x && pt.y < _size.y && pt.z < _size.z);
        return _data[pt.x + _size.x * (pt.y + _size.y * pt.z)];
    }

    template <typename T>
    const T& Array<T, 3>::operator()(const Point3UI &pt) const {
        JET_ASSERT(pt.x < _size.x && pt.y < _size.y && pt.z < _size.z);
        return _data[pt.x + _size.x * (pt.y + _size.y * pt.z)];
    }

    template <typename T>
    Array<T, 3>& Array<T, 3>::operator=(const T& value) {
        Set(value);
        return *this;
    }

    template <typename T>
    Array<T, 3>& Array<T, 3>::operator=(const Array& other) {
        Set(other);
        return *this;
    }

    template <typename T>
    Array<T, 3>& Array<T, 3>::operator=(
        const std::initializer_list<
            std::initializer_list<std::initializer_list<T>>>& list) {
        set(list);
        return *this;
    }

    template <typename T>
    Array<T, 3>::operator ArrayAccessor3<T>() {
        return Accessor();
    }

    template <typename T>
    Array<T, 3>::operator ConstArrayAccessor3<T>() const {
        return ConstAccessor();
    }
    
}
