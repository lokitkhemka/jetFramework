#pragma once


#if defined(DEBUG) || defined(_DEBUG)
#   define JET_DEBUG_MODE
#   include <cassert>
#   define JET_ASSERT(x) assert(x)
#else
#   define JET_ASSERT(x)
#endif


#if defined(_WIN32) || defined(_WIN64)
    #define JET_WINDOWS
#endif

#ifdef JET_WINDOWS
    #include <BaseTsd.h>
    typedef SSIZE_T ssize_t;
#endif