#pragma once

#include <Arrays/array_accessor.h>
#include <utility>


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
    {};
}