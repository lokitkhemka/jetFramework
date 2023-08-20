#pragma once

#include <cstddef>

namespace jet{

    //!
    //! \brief Generic N-dimensional array accessor class interface, where N=1,2 or 3
    //! 
    //! This class provides generic template class for N-dimensional array accessor
    //! where N must be 1, 2 or 3. This cannot create any instance by itself.
    //!
    //! Array accessor provides array-like data read/write functions, but does not
    //! handle memory management
    //! Parameters:
    //! \tparam T - Real Number type, i.e., float or double
    //! \tparam N - Dimension
    //!
    
    template<typename T, size_t N>
    class ArrayAccessor final{
    public:
        static_assert(N<1 || N>3 , "Not Implemented- N should be 1, 2 or 3.");
    };

    //!
    //! \brief Generic N-dimensional read-only array accessor class interface.
    //!
    
    template<typename T, size_t N>
    class ConstArrayAccessor final
    {
    public:
        static_assert(N<1 || N>3, "Not implemented- N should be either 1, 2, or 3");
    };
}