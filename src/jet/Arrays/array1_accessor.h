#pragma once

#include <macros.h>
#include<parallel.h>
#include<constants.h>
#include "Arrays/array_accessor.h"

namespace jet
{
    //!
    //! \brief 1-D array accessor class.
    //!
    //! This class represents 1-D array accessor. Array accessor provides
    //! array-like data read/write functions, but does not handle memory
    //! management.
    //!
    //!
    //! \see Array<T,2>
    //! \tparam T - Array Type Value
    
    template<typename T>
    class ArrayAccessor<T,1> final
    {
    public:
        //! Constructs empty 1-D array accessor
        ArrayAccessor();

        //! Construct an array accessor that wraps given array.
        ArrayAccessor(size_t size, T* const data);

        //! Copy Constructor.
        ArrayAccessor(const ArrayAccessor& other);

        //! Replaces teh content with given \p other array accessor.
        void Set(const ArrayAccessor& other);

        //! Resets the array.
        void Reset(size_t size, T* const data);

        //! Returns the reference to the i-th element.
        T& At(size_t i);

        //! Returns the const reference to the i-th element.
        const T& At(size_t i) const;

        //! Returns the begin iterator of the array.
        T* const begin() const;

        //! Returns the end iterator of the array.
        T* const end() const;

        //! Returns the end iterator of the array.
        T* begin();

        //! Returns the end iterator of the array.
        T* end();

        //! Returns the size of the array.
        size_t Size() const;

        //! Returns the raw pointer to the array data.
        T* const Data() const;

        //! Swaps the content of the Array Accessor with \p other Array Accessor.
        void Swap(ArrayAccessor& other);

        //!
        //! \brief Iterates the array and invoke the give \p func for each element.
        //!
        //! This function iterates the array elements and invoke the callback
        //! function \p func. The callback function takes one parameter which is the
        //! index of the array. The order of execution will be from 0 to N-1, where
        //! N is the size of the array.
        //!
        //! \code{.cpp}
        //! int data = {1,2,3,4,5,6};
        //! ArrayAccessor<int, 1> acc(6, data);
        //! acc.ForEach([](int elem) {
        //!     printf("%d\n", elem);
        //! });
        //! \endcode
        //!
        template <typename Callback>
        void ForEach(Callback func) const;
        

        //!
        //! \brief Iterates the array and invoke the give \p func for each index.
        //!
        //! This function iterates the array elements and invoke the callback
        //! function \p func. The callback function takes one parameter which is the
        //! index of the array. The order of execution will be from 0 to N-1, where
        //! N is the size of the array.
        //!
        //! \code{.cpp}
        //! int data = {1,2,3,4,5,6};
        //! ArrayAccessor<int, 1> acc(6, data);
        //! acc.ForEachIndex([&](size_t i) {
        //!     acc[i] = 4.f * i + 1.5f;
        //! });
        //! \endcode
        //!
        template <typename Callback>
        void ForEachIndex(Callback func) const;


        //! \brief Iterates the array and invoke given \p func for each element in
        //!     parallel using multi-threading.
        //!
        //! This function iterates the array elements and invoke the callback
        //! function \p func in parallel using multi-threading. The callback
        //! function takes array's element as its input. The order of execution will
        //! be non-deterministic since it runs in parallel.
        //! Below is the sample usage:
        //!
        //! \code{.cpp}
        //! int data = {1, 2, 3, 4, 5, 6};
        //! ArrayAccessor<int, 1> acc(6, data);
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
        //! function takes one parameter which is the index of the array. The order
        //! of execution will be non-deterministic since it runs in parallel.
        //! Below is the sample usage:
        //!
        //! \code{.cpp}
        //! int data = {1, 2, 3, 4, 5, 6};
        //! ArrayAccessor<int, 1> acc(6, data);
        //! acc.ParallelForEachIndex([](size_t i) {
        //!     acc[i] *= 2;
        //! });
        //! \endcode
        //!
        template <typename Callback>
        void ParallelForEachIndex(Callback func) const;


        //! Returns the reference of the i-th element.
        T& operator[](size_t i);

        //! Returns the const reference to i-th element.
        const T& operator[](size_t i) const;

        //! Copies given array accessor \p other
        ArrayAccessor& operator=(const ArrayAccessor& other);

        //! Casts the type to ConstArrayAccessor.
        operator ConstArrayAccessor<T,1>() const;

    private:
        size_t _size;
        T* _data;

    };

    //! Type alisas for 1-D array Accessor.
    template <typename T> using ArrayAccessor1 = ArrayAccessor<T,1>;


    //!
    //! \brief 1-D read-only array accessor class.
    //!
    //! This class represents 1-D read-only array accessor. Array accessor provides
    //! array-like data read/write functions, but does not handle memory
    //! management.
    
    template<typename T>
    class ConstArrayAccessor<T,1>
    {
    public:
        //! Constructs empty 1-D array accessor.
        ConstArrayAccessor();

        //! Constructs an read-only array accessor that wraps given array.
        ConstArrayAccessor(size_t size, const T* const data);

        //! Constructs a read-only array accesdsor from read/write accessor.
        explicit ConstArrayAccessor(const ArrayAccessor<T,1>& other);

        //! Copy Constructor.
        ConstArrayAccessor(const ConstArrayAccessor& other);

        //! Returns the const reference to the i-th element.
        const T& At(size_t i) const;

        //! Returns the begin iterator of the array.
        const T* const begin() const;

        //! Returns the end iterator of the array.
        const T* const end() const;

        //! Returns size of the array.
        size_t Size() const;

        //! Returns the raw pointer to the array data.
        const T* const Data() const;

        //!
        //! \brief Iterates the array and invoke given \p func for each element.
        //!
        //! This function iterates the array elements and invoke the callback
        //! function \p func. The callback function takes array's element as its
        //! input. The order of execution will be 0 to N-1 where N is the size of
        //! the array. Below is the sample usage:
        //!
        //! \code{.cpp}
        //! int data = {1, 2, 3, 4, 5, 6};
        //! ConstArrayAccessor<int, 1> acc(6, data);
        //! acc.ForEach([](int elem) {
        //!     printf("%d\n", elem);
        //! });
        //! \endcode

        template<typename Callback>
        void ForEach(Callback func) const;

        //!
        //! \brief Iterates the array and invoke given \p func for each index.
        //!
        //! This function iterates the array elements and invoke the callback
        //! function \p func. The callback function takes one parameter which is the
        //! index of the array. The order of execution will be 0 to N-1 where N is
        //! the size of the array. Below is the sample usage:
        //!
        //! \code{.cpp}
        //! int data = {1, 2, 3, 4, 5, 6};
        //! ConstArrayAccessor<int, 1> acc(6, data);
        //! acc.ForEachIndex([&](size_t i) {
        //!     data[i] = acc[i] * acc[i];
        //! });
        //! \endcode
        //!
        template <typename Callback>
        void ForEachIndex(Callback func) const;


        //! \brief Iterates the array and invoke given \p func for each index in
        //!     parallel using multi-threading.
        //!
        //! This function iterates the array elements and invoke the callback
        //! function \p func in parallel using multi-threading. The callback
        //! function takes one parameter which is the index of the array. The order
        //! of execution will be non-deterministic since it runs in parallel.
        //! Below is the sample usage:
        //!
        //! \code{.cpp}
        //! int data = {1, 2, 3, 4, 5, 6};
        //! ConstArrayAccessor<int, 1> acc(6, data);
        //! accessor.ParallelForEachIndex([](size_t i) {
        //!     data[i] = acc[i] * acc[i];
        //! });
        //! \endcode
        //!
        template <typename Callback>
        void ParallelForEachIndex(Callback func) const;


        //! Returns the const reference to i-th element.
        const T& operator[](size_t i) const;


    private:
        size_t _size;
        const T* _data;

    };


    //! Type alias for 1-D const array accessor.
    template <typename T> using ConstArrayAccessor1 = ConstArrayAccessor<T,1>;


    /**
     * IMPLEMENTATIONS OF THE METHODS OF ARRAYACCESSOR AND CONSTARRAYACCESSOR CLASSES.
     * 
     */

    template<typename T>
    ArrayAccessor<T,1>::ArrayAccessor() : _size(0), _data(nullptr){}

    template<typename T>
    ArrayAccessor<T,1>::ArrayAccessor(size_t size, T* const data)
    {
        Reset(size, data);
    }

    template<typename T>
    ArrayAccessor<T,1>::ArrayAccessor(const ArrayAccessor& other)
    {
        Set(other);
    }

    template<typename T>
    void ArrayAccessor<T,1>::Set(const ArrayAccessor& other)
    {
        Reset(other._size, other._data);
    }

    template<typename T>
    void ArrayAccessor<T,1>::Reset(size_t size, T* const data)
    {
        _size = size;
        _data = data;
    }

    template<typename T>
    T& ArrayAccessor<T,1>::At(size_t i)
    {
        JET_ASSERT(i< _size);
        return _data[i];
    }

    template<typename T>
    const T& ArrayAccessor<T,1>::At(size_t i) const
    {
        JET_ASSERT(i < _size);
        return _data[i];
    }

    template <typename T>
    T* const ArrayAccessor<T,1>::begin() const
    {
        return _data;
    }

    template<typename T>
    T* const ArrayAccessor<T,1>::end() const
    {
        return _data + _size;
    }

    template <typename T>
    T* ArrayAccessor<T,1>::begin()
    {
        return _data;
    }

    template<typename T>
    T* ArrayAccessor<T,1>::end()
    {
        return _data + _size;
    }

    template<typename T>
    size_t ArrayAccessor<T,1>::Size() const
    {
        return _size;
    }

    template<typename T>
    T* const ArrayAccessor<T,1>::Data() const
    {
        return _data;
    }

    template<typename T>
    void ArrayAccessor<T,1>::Swap(ArrayAccessor& other)
    {
        std::swap(other._data, _data);
        std::swap(other._size, _size);
    }

    template<typename T>
    template<typename Callback>
    void ArrayAccessor<T,1>::ForEach(Callback func) const
    {
        for (size_t i=0; i<Size(); ++i)
        {
            func(At(i));
        }
    }

    template<typename T>
    template<typename Callback>
    void ArrayAccessor<T,1>::ForEachIndex(Callback func) const
    {
        for (size_t i=0; i<Size(); ++i)
            func(i);
    }

    template <typename T>
    template <typename Callback>
    void ArrayAccessor<T, 1>::ParallelForEach(Callback func) {
        ParallelFor(kZeroSize, Size(), [&](size_t i) {
            func(At(i));
        });
    }

    template <typename T>
    template <typename Callback>
    void ArrayAccessor<T, 1>::ParallelForEachIndex(Callback func) const {
        ParallelFor(kZeroSize, Size(), func);
    }

    template<typename T>
    T& ArrayAccessor<T,1>::operator[](size_t i){
        return _data[i];
    }

    template<typename T>
    const T& ArrayAccessor<T,1>::operator[](size_t i) const
    {
        return _data[i];
    }

    template<typename T>
    ArrayAccessor<T,1>&
    ArrayAccessor<T,1>::operator=(const ArrayAccessor& other)
    {
        Set(other);
        return *this;
    }

    template<typename T>
    ArrayAccessor<T,1>::operator ConstArrayAccessor<T,1>() const
    {
        return ConstArrayAccessor<T,1>(*this);
    }


    template <typename T>
    ConstArrayAccessor<T,1>::ConstArrayAccessor() : _size(0), _data(nullptr){}

    template <typename T>
    ConstArrayAccessor<T, 1>::ConstArrayAccessor(
        size_t size, const T* const data) {
        _size = size;
        _data = data;
    }

    template <typename T>
    ConstArrayAccessor<T, 1>::ConstArrayAccessor(const ArrayAccessor<T, 1>& other) {
        _size = other.Size();
        _data = other.Data();
    }

    template <typename T>
    ConstArrayAccessor<T, 1>::ConstArrayAccessor(const ConstArrayAccessor& other) {
        _size = other._size;
        _data = other._data;
    }

    template <typename T>
    const T& ConstArrayAccessor<T, 1>::At(size_t i) const {
        JET_ASSERT(i < _size);
        return _data[i];
    }

    template <typename T>
    const T* const ConstArrayAccessor<T, 1>::begin() const {
        return _data;
    }

    template <typename T>
    const T* const ConstArrayAccessor<T, 1>::end() const {
        return _data + _size;
    }

    template <typename T>
    size_t ConstArrayAccessor<T, 1>::Size() const {
        return _size;
    }

    template <typename T>
    const T* const ConstArrayAccessor<T, 1>::Data() const {
        return _data;
    }

    template <typename T>
    template <typename Callback>
    void ConstArrayAccessor<T, 1>::ForEach(Callback func) const {
        for (size_t i = 0; i < Size(); ++i) {
            func(At(i));
        }
    }

    template <typename T>
    template <typename Callback>
    void ConstArrayAccessor<T, 1>::ForEachIndex(Callback func) const {
        for (size_t i = 0; i < Size(); ++i) {
            func(i);
        }
    }

    template <typename T>
    template <typename Callback>
    void ConstArrayAccessor<T, 1>::ParallelForEachIndex(Callback func) const {
        ParallelFor(kZeroSize, Size(), func);
    }

    template <typename T>
    const T& ConstArrayAccessor<T, 1>::operator[](size_t i) const {
        return _data[i];
    }


} // namespace jet
