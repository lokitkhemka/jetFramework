#pragma once

#include <macros.h>
#include <array>
#include <type_traits>

#include <cassert>

namespace jet
{

    //!
    //! \brief Generic N-dimensional Point Class
    //!
    //! \tparam T - Number Type
    //! \tparam N - Dimension
    //!

    template<typename T, size_t N>
    class Point final
    {
    public:
        static_assert( N>0, "Size of the static-sized point should be greater than zero.");
        static_assert(std::is_arithmetic<T>::value, "Point can only be instantiated with arithmetic types.");

        //! Constructs a point with zeros
        Point();

        //! Construct Point instance with parameters.
        template<typename... Params>
        explicit Point(Params... params);

        //! Copy Constructor
        Point(const Point& other);

        //! Constructs Point instance with initializer list
        template <typename U>
        explicit Point(const std::initializer_list<U>& list);

        //! Set Point instance with initializer list.
        template<typename U>
        void Set(const std::initializer_list<U>& list);

        //! Set Point instance with other point.
        void Set(const Point& other);

        //! Set Point instance with initializer list
        template<typename U>
        Point& operator=(const std::initializer_list<U>& list);

        //! Set point instance with other point.
        Point& operator=(const Point& other);

        //! Returns the const reference to the \p i-th element.
        const T& operator[](size_t i) const;

        //! Returns the reference to the \p i-th element.
        T& operator[](size_t i);

    private:
        std::array<T,N> _elements;

        template <typename... Params>
        void SetAt(size_t i, T v, Params... params);

        void SetAt(size_t i, T v);

    };

    template <typename T, size_t N>
    Point<T,N>::Point()
    {
        for (auto& elem: _elements)
        {
            elem = static_cast<T>(0);
        }
    }

    template<typename T, size_t N>
    template <typename... Params>
    Point<T,N>::Point(Params... params)
    {
        static_assert(sizeof...(params)==N, "Invalid number of parameters.");

        SetAt(0, params...);
    }

    template<typename T, size_t N>
    template<typename U>
    Point<T,N>::Point(const std::initializer_list<U>& list)
    {
        JET_ASSERT(list.size() >= N);

        size_t i = 0;
        for (const auto& InputElem: list)
        {
            _elements[i] = static_cast<T>(InputElem);
            ++i;
        }
    }

    template<typename T, size_t N>
    Point<T,N>::Point(const Point& other)
                    :
                    _elements(other._elements)
    {}


    template <typename T, size_t N>
    template <typename U>
    void Point<T, N>::Set(const std::initializer_list<U>& list)
    {
        assert(list.size() >= N);

        size_t i = 0;
        for(const auto& InputElem : list)
        {
            _elements[i] = static_cast<T>(InputElem);
            ++i;
        }
    }

    template<typename T, size_t N>
    void Point<T,N>::Set(const Point& other)
    {
        _elements = other._elements;
    }

    template<typename T, size_t N>
    template <typename U>
    Point<T,N>& Point<T,N>::operator=(const std::initializer_list<U>& list)
    {
        Set(list);
        return *this;
    }

    template<typename T, size_t N>
    Point<T,N>& Point<T,N>::operator=(const Point& other)
    {
        Set(other);
        return *this;
    }

    template<typename T, size_t N>
    const T& Point<T,N>::operator[](size_t i) const
    {
        return _elements[i];
    }

    template<typename T, size_t N>
    T& Point<T,N>::operator[](size_t i)
    {
        return _elements[i];
    }

    template<typename T, size_t N>
    template <typename... Params>
    void Point<T,N>::SetAt(size_t i, T v, Params... params)
    {
        _elements[i] = v;
        SetAt(i+1, params...);
    }

    template<typename T, size_t N>
    void Point<T,N>::SetAt(size_t i, T v)
    {
        _elements[i] = v;
    }

}
