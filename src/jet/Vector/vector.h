#pragma once

#include<constants.h>
#include<type_helpers.h>
#include<Arrays/array.h>
#include<type_traits>

#include <cassert>
#include <array>

namespace jet
{
    //!
    //! \brief Generic N-D vector class.
    //!
    //! \tparam T - Number Type.
    //! \tparam N - Dimension

    template<typename T, size_t N>
    class Vector final
    {
    public:
        static_assert( N > 0, "Size of the static-sized vector should be greater than zero.");
        static_assert( std::is_floating_point<T>::value, "Vector can only be instantiated with floating point types.");

        //! Constructs a vectors with zeros.
        Vector();

        //! Constructs vector instance with parameters.
        template <typename... Params>
        explicit Vector(Params... params);

        //! Constructs vector isntance with initializer list.
        template<typename U>
        Vector(const std::initializer_list<U>& list);

        //! Copy constructor.
        Vector(const Vector& other);

        //Set Vector Instance with initializer list.
        template<typename U>
        void Set(const std::initializer_list<U>& list);

        //!  Set vector instance with other vector.
        void Set(const Vector& other);

        //! Set Vector instance with initializer list.
        template<typename U>
        Vector& operator=(const std::initializer_list<U>& list);

        //! Sets vector instance with other vector.
        Vector& operator=(const Vector& other);

        //! Returns the const reference to the \p i-th element.
        const T& operator[](size_t i) const;

        //! Returns the reference to the \p i-th element.
        T& operator[](size_t i);


    private:
        std::array<T,N> _elements;

        template<typename... Params>
        void SetAt(size_t i, T v, Params... params);
        void SetAt(size_t i, T v);
    };

    //! Returns the type of the value.
    template<typename T, size_t N>
    struct ScalarType<Vector<T, N>>
    {
        typedef T value;
    };


    template<typename T, size_t N>
    Vector<T,N>::Vector()
    {
        for (auto& elem: _elements)
        {
            elem = static_cast<T>(0);
        }
    }

    template<typename T, size_t N>
    template<typename... Params>
    Vector<T,N>::Vector(Params... params)
    {
        static_assert(sizeof...(params) == N, "Invalid Number of parameters.");
        SetAt(0, params...);
    }

    template<typename T, size_t N>
    template<typename U>
    Vector<T,N>::Vector(const std::initializer_list<U>& list)
    {
        Set(list);
    }

    template<typename T, size_t N>
    Vector<T,N>::Vector(const Vector& other):
        _elements(other._elements)
    {}

    template<typename T, size_t N>
    template<typename U>
    void Vector<T,N>::Set(const std::initializer_list<U>& list)
    {
        assert(list.size() >= N);

        size_t i = 0;
        for (const auto& InputElem : list)
        {
            _elements[i] = static_cast<T>(InputElem);
            ++i;
        }
    }

    template<typename T, size_t N>
    void Vector<T,N>::Set(const Vector& other)
    {
        _elements = other._elements;
    }

    template<typename T, size_t N>
    template<typename U>
    Vector<T,N>& Vector<T,N>::operator=(const std::initializer_list<U>& list)
    {
        Set(list);
        return (*this);
    }

    template<typename T, size_t N>
    Vector<T,N>& Vector<T,N>::operator=(const Vector& other)
    {
        Set(other);
        return *this;
    }

    template<typename T, size_t N>
    const T& Vector<T,N>::operator[](size_t i) const
    {
        return _elements[i];
    }

    template<typename T, size_t N>
    T& Vector<T,N>::operator[](size_t i)
    {
        return _elements[i];
    }

    template<typename T, size_t N>
    template <typename... Params>
    void Vector<T,N>::SetAt(size_t i, T v, Params... params)
    {
        _elements[i] = v;
        SetAt(i+1, params...);
    }

    template<typename T, size_t N>
    void Vector<T,N>::SetAt(size_t i, T v)
    {
        _elements[i] = v;
    }
}