#pragma once


#if defined(DEBUG) || defined(_DEBUG)
#   define JET_DEBUG_MODE
#   include <cassert>
#   define JET_ASSERT(x) assert(x)
#else
#   define JET_ASSERT(x)
#endif