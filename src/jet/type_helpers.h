#pragma once

namespace jet
{
    //! Returns the type of the value itself.
    template<typename T>
    struct ScalarType
    {
        typedef T value;
    };
}