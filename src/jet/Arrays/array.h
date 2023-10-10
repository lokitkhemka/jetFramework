#pragma once
#include <stdio.h>
namespace jet{
    //! \brief Array class provides a Generic N-dimension array class interface, where N = 1,2,3
    //!
    //!
    //! This class provides generic template class for N-dimensional array where N = 1,2 or 3.
    //! This class cannot create any instance by itself.
    //!
    //! Paramters:
    //! \tparam T - Real Number type (float or double, etc.)
    //! \tparam N - Dimension.
    //!
    
    template <typename T, size_t N>
    class Array final{
    public:
        static_assert( N < 1 || N > 3, "N should be either 1, 2, or 3");
    };
}
