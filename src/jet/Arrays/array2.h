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
    private:
        Size2 _size;
        std::vector<T> _data;
    };

    template<typename T> using Array2 = Array<T,2>;
}
