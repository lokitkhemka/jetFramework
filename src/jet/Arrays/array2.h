#pragma once

#include<Arrays/array.h>
#include<Arrays/array2_accessor.h>
#include<Size/size2.h>
#include<fstream>
#include<functional>
#include<iostream>
#include<utility>
#include<vector>


namespace jet
{
    //! \brief 2-D array class.
    //!
    //! This class represents 2-D array data structure. Internally, the 2-D data is
    //! mapped to a linear array such that (i, j) element is actually stroed at
    //! (i + (width * j))th element of the linear array. This mapping means
    //! iterating i first and j next will give better performance such as:
    //!
    //! \code{.cpp}
    //! Array<int, 2> array;
    //! for (size_t j = 0; j < array.height(); ++j) {
    //!     for (size_t i = 0; i < array.width(); ++i) {
    //!         // Read or write array(i, j)
    //!     }
    //! }
    //! \endcode
    //!
    //! \tparam T - Type to store in the array.
    template<typename T>
    class Array<T,2> final
    {
    public:
        typedef std::vector<T> ContainerType;

        //! Constructs zero-sized 2D array
        Array();

        //! Constructs 2D array with given \p size and fill it with \p initVal.
        explicit Array(const Size2& size, const T& initVal=T());

        //! Constructs 2D array with size \p width times \p height and fill it with \p initVal.
        Array(size_t widht, size_t height, const T& initVal = T());

        //! \brief Constructs 2-D array with given initializer list \p list.
        //!
        //! This constructor will build 2-D array with given initializer list \p list
        //! such as
        //!
        //! \code{.cpp}
        //! Array<int, 2> arr = {
        //!     {1, 2, 4},
        //!     {9, 3, 5}
        //! };
        //! \endcode
        //!
        //! Note the initializer also has 2-D structure. The code above will
        //! construct 3 x 2 array.
        //!
        //! \param lst Initializer list that should be copy to the new array.
        //!
        Array(const std::initializer_list<std::initializer_list<T>>& lst);

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
        //! Array<int, 2> arr;
        //! arr = {
        //!     {1, 2, 4},
        //!     {9, 3, 5}
        //! };
        //! \endcode
        //!
        //! Note the initializer also has 2-D structure. The code above will
        //! build 3 x 2 array.
        //!
        //! \param lst Initializer list that should be copy to the new array.
        //!
        void Set(const std::initializer_list<std::initializer_list<T>>& lst);

        //!Clears the array and resizes to zero.
        void Clear();

        //! Resizes the array with \p size and fill the new elements with \p initVal.
        void Resize(const Size2& size, const T& initVal=T());

        //! Resizes the array with size \p width x \p height and fill the new
        //! element with \p initVal.
        void Resize(size_t width, size_t height, const T& initVal = T());

        //! Returns the reference to the i-th element, where i = x + (width*y)
        T& At(size_t i);

        //! Returns the const reference to the i-th element, where i = x+(width*y)
        const T& At(size_t i) const;

        //! Returns the reference to the element at (pt.x, pt.y)
        T& At(const Point2UI& pt);

        //! Returns the const reference to the element at (pt.x, pt.y)
        const T& At(const Point2UI& pt) const;

        //! Returns the reference to the element at (i,j).
        T& At(size_t i, size_t j);

        //! Returns the const reference to the element at (i,j).
        const T& At(size_t i, size_t j) const;

        //! Returns the size of the array.
        Size2 Size() const;

        //! Returns the Width of the array.
        size_t Width() const;

        //! Returns the Height of the array.
        size_t Height() const;

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
        ArrayAccessor2<T> Accessor();

        //! Returns the const array accessor.
        ConstArrayAccessor2<T> ConstAccessor() const;

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
        //! Array<int, 2> array;
        //! for (size_t j = 0; j < array.height(); ++j) {
        //!     for (size_t i = 0; i < array.width(); ++i) {
        //!         func(array(i, j));
        //!     }
        //! }
        //! \endcode
        //!
        //! Below is the sample usage:
        //!
        //! \code{.cpp}
        //! Array<int, 2> array(100, 200, 4);
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
        //! function \p func. The callback function takes two parameters which are
        //! the (i, j) indices of the array. The order of execution will be the same
        //! as the nested for-loop below:
        //!
        //! \code{.cpp}
        //! Array<int, 2> array;
        //! for (size_t j = 0; j < array.height(); ++j) {
        //!     for (size_t i = 0; i < array.width(); ++i) {
        //!         func(i, j);
        //!     }
        //! }
        //! \endcode
        //!
        //! Below is the sample usage:
        //!
        //! \code{.cpp}
        //! Array<int, 2> array(10, 20, 4);
        //! array.ForEachIndex([&](size_t i, size_t j) {
        //!     array(i, j) = 4.f * i + 7.f * j + 1.5f;
        //! });
        //! \endcode
        //!
        template <typename Callback>
        void ForEachIndex(Callback func) const;

        ///TODO: Implement Parallel foreach methods


        //! Returns the reference to the i-th element, where i = x + (width * y)
        //! \see Array<T,2>::At
        T& operator[](size_t i);

        //! Returns the const reference to the i-th element, where i = x+(width * y)
        //! \see Array<T,2>::At
        const T& operator[](size_t i) const;

        //! Returns the reference to the element at (i,j)
        T& operator()(size_t i, size_t j);

        //! Returns the const reference to the element at (i,j)
        const T& operator()(size_t i, size_t j) const;

        //! Returns the reference to the element at (pt.x, pt.y)
        T& operator()(const Point2UI& pt);

        //! Returns the const reference to the element at (pt.x, pt.y)
        const T& operator()(const Point2UI& pt) const;

        //! Sets the array with given \p value.
        Array& operator=(const T& other);

        //! Copies given array \p other to this array.
        Array& operator=(const Array& other);


        //! Copies given initializer list \p list to this array.
        //!
        //! This function copies given initializer list \p list to the array such as
        //!
        //! \code{.cpp}
        //! Array<int, 2> arr;
        //! arr = {
        //!     {1, 2, 4},
        //!     {9, 3, 5}
        //! };
        //! \endcode
        //!
        //! Note the initializer also has 2-D structure. The code above will
        //! build 3 x 2 array.
        //!
        //! \param list Initializer list that should be copy to the new array.
        //!
        Array& operator=(
            const std::initializer_list<std::initializer_list<T>>& list);


        //! Casts to array accessor.
        operator ArrayAccessor2<T>();

        //! Casts to const array accessor.
        operator ConstArrayAccessor2<T>() const;

    private:
        Size2 _size;
        std::vector<T> _data;
    };

    template<typename T> using Array2 = Array<T,2>;

    template<typename T>
    Array<T,2>::Array()
    {}

    template<typename T>
    Array<T,2>::Array(const Size2& size, const T& initVal)
    {
        Resize(size, initVal);
    }

    template<typename T>
    Array<T,2>::Array(size_t width, size_t height, const T& initVal)
    {
        Resize(width, height, initVal);
    }

    template<typename T>
    Array<T,2>::Array(const std::initializer_list<std::initializer_list<T>>& list)
    {
        Set(list);
    }

    template<typename T>
    Array<T,2>::Array(const Array& other)
    {
        Set(other);
    }

    template<typename T>
    void Array<T,2>::Set(const T& value)
    {
        for (auto& v : _data)
        {
            v = value;
        }
    }

    template<typename T>
    void Array<T,2>::Set(const Array& other)
    {
        _data.resize(other._data.size());
        std::copy(other._data.begin(), other._data.end(), _data.begin());
        _size = other._size;
    }

    template<typename T>
    void Array<T,2>::Set(const std::initializer_list<std::initializer_list<T>>& list)
    {
        size_t height = list.size();
        size_t width = (height > 0) ? list.begin()->size() : 0;
        Resize(Size2(width, height));
        auto rowIter = list.begin();
        for (size_t j = 0; j< height; ++j)
        {
            JET_ASSERT(width == rowIter->size());
            auto colIter = rowIter->begin();
            for (size_t i = 0; i< width; ++i)
            {
                (*this)(i,j) = *colIter;
                ++colIter;
            }
            ++rowIter;
        }
    }

    template<typename T>
    void Array<T,2>::Clear()
    {
        _data.clear();
        _size = Size2(0,0);
    }

    template<typename T>
    void Array<T,2>::Resize(const Size2& size, const T& initVal)
    {
        Array grid;
        grid._data.resize(size.x * size.y, initVal);
        grid._size = size;
        size_t iMin = std::min(size.x, _size.x);
        size_t jMin = std::min(size.y, _size.y);
        for(size_t j = 0; j<jMin; ++j)
        {
            for (size_t i = 0; i < iMin; ++i)
            {
                 grid(i,j) = At(i,j);
            }
        }
        Swap(grid);
    }

    template<typename T>
    void Array<T,2>::Resize(size_t width, size_t height, const T& initVal)
    {
        Resize(Size2(width,height), initVal);
    }

    template<typename T>
    T& Array<T,2>::At(size_t i)
    {
        JET_ASSERT(i< _size.x * _size.y);
        return _data[i];
    }

    template<typename T>
    const T& Array<T,2>::At(size_t i) const
    {
        JET_ASSERT(i < _size.x * _size.y);
        return _data[i];
    }

    template<typename T>
    T& Array<T,2>::At(const Point2UI& pt)
    {
        return At(pt.x, pt.y);
    }

    template<typename T>
    const T& Array<T,2>::At(const Point2UI& pt) const
    {
        return At(pt.x, pt.y);
    }

    template<typename T>
    T& Array<T,2>::At(size_t i, size_t j)
    {
        JET_ASSERT(i < _size.x && j < _size.y);
        return _data[i + _size.x * j];
    }

    template<typename T>
    const T& Array<T,2>::At(size_t i, size_t j) const
    {
        JET_ASSERT(i < _size.x && j < _size.y);
        return _data[i + _size.x * j];
    }

    template<typename T>
    Size2 Array<T,2>::Size() const
    {
        return _size;
    }

    template<typename T>
    size_t Array<T,2>::Width() const
    {
        return _size.x;
    }

    template<typename T>
    size_t Array<T,2>::Height() const
    {
        return _size.y;
    }

    template<typename T>
    T* Array<T,2>::Data()
    {
        return _data.data();
    }

    template<typename T>
    const T* const Array<T,2>::Data() const
    {
        return _data.data();
    }

    template<typename T>
    typename Array<T,2>::ContainerType::iterator Array<T,2>::begin()
    {
        return _data.begin();
    }

    template<typename T>
    typename Array<T,2>::ContainerType::const_iterator Array<T,2>::begin() const
    {
        return _data.begin();
    }

    template<typename T>
    typename Array<T,2>::ContainerType::iterator Array<T,2>::end()
    {
        return _data.end();
    }

    template<typename T>
    typename Array<T,2>::ContainerType::const_iterator Array<T,2>::end() const
    {
        return _data.end();
    }

    template<typename T>
    ArrayAccessor2<T> Array<T,2>::Accessor()
    {
        return ArrayAccessor2<T>(Size(), Data());
    }

    template<typename T>
    ConstArrayAccessor2<T> Array<T,2>::ConstAccessor() const
    {
        return ConstArrayAccessor2<T>(Size(), Data());
    }

    template<typename T>
    void Array<T,2>::Swap(Array& other)
    {
        std::swap(other._data, _data);
        std::swap(other._size, _size);
    }

    template <typename T>
    template <typename Callback>
    void Array<T, 2>::ForEach(Callback func) const {
        ConstAccessor().ForEach(func);
    }

    template <typename T>
    template <typename Callback>
    void Array<T, 2>::ForEachIndex(Callback func) const {
        ConstAccessor().ForEachIndex(func);
    }





    template <typename T>
    T& Array<T, 2>::operator[](size_t i) {
        return _data[i];
    }

    template <typename T>
    const T& Array<T, 2>::operator[](size_t i) const {
        return _data[i];
    }

    template <typename T>
    T& Array<T, 2>::operator()(size_t i, size_t j) {
        JET_ASSERT(i < _size.x && j < _size.y);
        return _data[i + _size.x * j];
    }

    template <typename T>
    const T& Array<T, 2>::operator()(size_t i, size_t j) const {
        JET_ASSERT(i < _size.x && j < _size.y);
        return _data[i + _size.x * j];
    }

    template <typename T>
    T& Array<T, 2>::operator()(const Point2UI &pt) {
        JET_ASSERT(pt.x < _size.x && pt.y < _size.y);
        return _data[pt.x + _size.x * pt.y];
    }

    template <typename T>
    const T& Array<T, 2>::operator()(const Point2UI &pt) const {
        JET_ASSERT(pt.x < _size.x && pt.y < _size.y);
        return _data[pt.x + _size.x * pt.y];
    }

    template <typename T>
    Array<T, 2>& Array<T, 2>::operator=(const T& value) {
        Set(value);
        return *this;
    }

    template <typename T>
    Array<T, 2>& Array<T, 2>::operator=(const Array& other) {
        Set(other);
        return *this;
    }

    template <typename T>
    Array<T, 2>& Array<T, 2>::operator=(
        const std::initializer_list<std::initializer_list<T>>& lst) {
        Set(lst);
        return *this;
    }

    template <typename T>
    Array<T, 2>::operator ArrayAccessor2<T>() {
        return Accessor();
    }

    template <typename T>
    Array<T, 2>::operator ConstArrayAccessor2<T>() const {
        return ConstAccessor();
    }
    
}
