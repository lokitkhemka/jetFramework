#pragma once
#include "array.h"
#include "array1_accessor.h"

#include <fstream>
#include <functional>
#include <iostream>
#include <utility>
#include <vector>


namespace jet{

    template <typename T>
    class Array<T, 1> final{
    public:
        typedef std::vector<T> ContainerType;

        //! Constructs a Zero-sized 1D array
        Array();



        //! Constructs 1-D array with given \p size and ifll it with \p initVal
        //! Parameters:
        //! \param size Initial size of the array
        //! \param initVal Initial value of each array element.
        explicit Array(size_t size, const T& initVal = T());


        //! \brief Constructs 1-D array with given initializer list \p list
        //!
        //! This constructor will 1-D array with given initializer list \p list
        //!
        //!
        //! \code{.cpp}
        //! Array<int,1> arr = {1,2,4,5,6};
        //! \endcode
        //! 
        //! Parameters:
        //! \param list Initializer list that should be copied to the new array

        Array(const std::initializer_list<T>&list);


        //! Copy Constructor
        Array(const Array& other);

        //! Set the entire array with given \p value
        void Set(const T& value);

        //! Copies the array \p other to this array.
        void Set(const Array& other);

        //! Copies given initializer list \p list to this array
        void Set(const std::initializer_list<T>& list);

        //! Clears the array and resizes to zero
        void Clear();

        //! Resizes the array with \p size and fill the new element with \p initVal.
        void Resize(size_t size, const T& initVal = T());

        //! Returns the reference to the i-th element.
        T& At(size_t i);

        //! Returns the const reference to the i-th element.
        const T& At(size_t i) const;

        //! Returns the size of the array.
        size_t Size() const;

        //! Returns the raw pointer to the array data.
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

        //! Returns the array Accessor.
        ArrayAccessor1<T> Accessor();

        //! Returns the const array accessor.
        ConstArrayAccessor1<T> ConstAccessor() const;

        //! Swaps the content of the array with \p other array.
        void Swap(Array& other);

        //! Appends a single value \p newVal at the end of the array.
        void Append(const T& newVal);

        //! Appends \p other array at the end of the array.
        void Append(const Array& other);

        //! 
        //! \brief Iterates the array and invoke the give \p func for each element.
        //!
        //!
        //! This funciton iterates the array elements and invoke the callback function
        //! \p func. The callback function takes array's element as its input.
        //! The order of execution will be 0 to N-1 where N is the size of the array.
        //! Usage Example:
        //!
        //! \code{.cpp}
        //! Array<int,1> array(10,4);
        //! array.forEach([](int elem){
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
        //! function \p func. The callback function takes one parameter which is the
        //! index of the array. The order of execution will be 0 to N-1 where N is
        //! the size of the array. 
        //! Usage Example:
        //!
        //! \code{.cpp}
        //! Array<int, 1> array(10, 4);
        //! array.ForEachIndex([&](size_t i) {
        //!     array[i] = 4.f * i + 1.5f;
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
        //! Array<int, 1> array(1000, 4);
        //! array.ParallelForEach([](int& elem) {
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
        //! Array<int, 1> array(1000, 4);
        //! array.ParallelForEachIndex([](size_t i) {
        //!     array[i] *= 2;
        //! });
        //! \endcode
        //!
        template <typename Callback>
        void ParallelForEachIndex(Callback func) const;


        //! Returns the referece to i-th element.
        T& operator[](size_t i);

        //! Returns the const reference to i-th element.
        const T& operator[](size_t i) const;

        //!Sets the entire array with the given \p value
        Array& operator=(const T& value);

        //! Copies the given array \p other to this array.
        Array& operator=(const Array& other);

        //! Copies given initializer list \p list to this array.
        Array& operator=(const std::initializer_list<T>& list);

        //! Casts to ArrayAccessor
        operator ArrayAccessor1<T>();

        //! Casts to ConstArrayAccessor.
        operator ConstArrayAccessor1<T>() const;


    private:
        ContainerType _data;

    };

    //! Type alias for 1-D array
    template <typename T> using Array1 = Array<T,1>;

    /*
    IMPLEMENTATIONS OF THE METHODS OF Array<T,1> CLASS ARE BELOW.
    
    */

    template<typename T>
    Array<T, 1>::Array(){
    }


    template <typename T>
    Array<T, 1>::Array(size_t size, const T& initVal)
    {
        Resize(size, initVal);
    }

    template <typename T>
    Array<T,1>::Array(const std::initializer_list<T>& list)
    {
        Set(list);
    }

    template <typename T>
    Array<T,1>::Array(const Array& other)
    {
        Set(other);
    }

    template <typename T>
    void Array<T,1>::Set(const T& value)
    {
        for (auto& v:_data)
        {
            v = value;
        }
    }

    template<typename T>
    void Array<T,1>::Set(const Array& other)
    {
        _data.resize(other._data.size());
        std::copy(other._data.begin(), other._data.end(), _data.begin());
    }

    template<typename T>
    void Array<T,1>::Set(const std::initializer_list<T>& list)
    {
        size_t size = list.size();
        Resize(size);
        auto Iter = list.begin();
        for(size_t i =0; i<size; ++i)
        {
            (*this)[i] = *Iter;
            ++Iter;
        }
    }

    template <typename T>
    void Array<T,1>::Clear()
    {
        _data.clear();
    }

    template <typename T>
    void Array<T, 1>::Resize(size_t size, const T& initVal)
    {
        _data.resize(size, initVal);
    }

    template<typename T>
    T& Array<T,1>::At(size_t i)
    {
        assert(i < Size());
        return _data[i];
    }

    template<typename T>
    const T& Array<T,1>::At(size_t t) const
    {
        assert(i<Size());
        return _data[i];
    }

    template <typename T>
    size_t Array<T, 1>::Size() const {
        return _data.size();
    }
    
    template<typename T>
    T* Array<T,1>::Data()
    {
        return _data.data();
    }

    template<typename T>
    const T* const Array<T,1>::Data() const
    {
        return _data.data();
    }

    template<typename T>
    typename Array<T,1>::ContainerType::iterator Array<T,1>::begin()
    {
        return _data.begin();
    }

    template<typename T>
    typename Array<T,1>::ContainerType::const_iterator Array<T,1>::begin() const
    {
        return _data.begin();
    }

    template<typename T>
    typename Array<T,1>::ContainerType::iterator Array<T,1>::end()
    {
        return _data.end();
    }

    template<typename T>
    typename Array<T,1>::ContainerType::const_iterator Array<T,1>::end() const
    {
        return _data.end();
    }

    template <typename T>
    ArrayAccessor1<T> Array<T,1>::Accessor()
    {
        return ArrayAccessor1<T>(Size(), Data());
    }

    template<typename T>
    ConstArrayAccessor1<T> Array<T,1>::ConstAccessor() const
    {
        return ConstArrayAccessor1<T>(Size(), Data());
    }

    template<typename T>
    void Array<T,1>::Swap(Array& other)
    {
        std::swap(other._data, _data);
    }

    template<typename T>
    void Array<T,1>::Append(const T& newVal)
    {
        _data.push_back(newVal);
    }

    template<typename T>
    void Array<T,1>::Append(const Array& other)
    {
        _data.insert(_data.end(), other._data.begin(), other._data.end());
    }

    template <typename T>
    template <typename Callback>
    void Array<T,1>::ForEach(Callback func) const
    {
        ConstAccessor().ForEach(func);
    }

    template <typename T>
    template <typename Callback>
    void Array<T,1>::ForEachIndex(Callback func) const
    {
        ConstAccessor().ForEachIndex(func);
    }

    template <typename T>
    template <typename Callback>
    void Array<T, 1>::ParallelForEach(Callback func) {
        Accessor().ParallelForEach(func);
    }

    template <typename T>
    template <typename Callback>
    void Array<T, 1>::ParallelForEachIndex(Callback func) const {
        ConstAccessor().ParallelForEachIndex(func);
    }


    template<typename T>
    T& Array<T,1>::operator[](size_t i)
    {
        return _data[i];
    }

    template<typename T>
    const T& Array<T,1>::operator[](size_t i) const
    {
        return _data[i];
    }

    template<typename T>
    Array<T,1>& Array<T,1>::operator=(const T& value)
    {
        Set(value);
        return *this;
    }

    template<typename T>
    Array<T,1>& Array<T,1>::operator=(const Array& other)
    {
        Set(other);
        return *this;
    }

    template<typename T>
    Array<T,1>& Array<T,1>::operator=(const std::initializer_list<T>& list)
    {
        Set(list);
        return *this;
    }

    template<typename T>
    Array<T,1>::operator ArrayAccessor1<T>()
    {
        return Accessor();
    }

    template <typename T>
    Array<T,1>::operator ConstArrayAccessor1<T>() const
    {
        return ConstAccessor();
    }

}
