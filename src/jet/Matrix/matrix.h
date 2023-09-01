#pragma once

#include<macros.h>
#include<array>
#include<type_traits>
#include<cassert>

namespace jet{
    
    //! \brief Static-sized M x N matrix class.
    //!
    //!
    //! \tparam T - Real Number type.
    //! \tparam M - Number of rows.
    //! \tparam N - Number of columns.
    template<typename T, size_t M, size_t N>
    class Matrix
    {
    public:
        static_assert( M > 0, "Number of rows for the matrix should be greater than zero.");
        static_assert(N > 0, "Number of columns for the matrix should be greater than zero.");
        static_assert(std::is_floating_point<T>::value,
                    "Matrix can only be instantiated with floating point type");

        std::array<T, M*N> elements;
        
        //! Default Constructor
        Matrix();

    };

    template<typename T, size_t M, size_t N>
    Matrix<T,M,N>::Matrix()
    {
        for (auto& elem: elements)
        {
            elem = 0;
        }
    }
}