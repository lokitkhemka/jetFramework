#pragma once

#include<Vector/vector.h>

namespace jet
{
    //! \brief Base Class for ray.
    //!
    //! \tparam T The value type
    //! \tparam N The dimension
    template<typename T, size_t N>
    class Ray
    {
        static_assert(N != 2 && N !=3, "Not Implemented");
        static_assert(std::is_floating_point<T>::value, "Ray can only be instantiated with floating point value");
    };
}