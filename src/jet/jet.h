#pragma once

#include <macros.h>

#ifdef JET_WINDOWS
#   define WIN32_LEAN_AND_MEAN
#   define NOMINMAX
#   include <objbase.h>
#endif

#ifndef UNUSED_VARIABLE
#   define UNUSED_VARIABLE(x) ((void)x)
#endif

#ifndef CLONE_W_CUSTOM_DELETER
#   define CLONE_W_CUSTOM_DELETER(ClassName) \
        std::shared_ptr<ClassName>( \
            new ClassName(*this), \
            [] (ClassName* obj) { \
                delete obj; \
            });
#endif

#ifndef UNUSED_VARAIBLE
    #define UNUSED_VARAIBLE(x) ((void)x)
#endif
