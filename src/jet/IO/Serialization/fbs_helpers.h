#pragma once

#include "generated/basic_types_generated.h"
#include <Size/size2.h>
#include <Size/size3.h>
#include <Vector/vector2.h>
#include <Vector/vector3.h>
#include <algorithm>
#include <vector>


namespace jet
{
    inline fbs::Size2 JetToFbs(const Size2& vec)
    {
        return fbs::Size2(vec.x, vec.y);
    }
}