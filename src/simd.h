#pragma once

namespace simd
{
    struct DispatchBase
    {
        #define SIMD_REGISTER(RET,NAME,...) \
            virtual RET NAME(__VA_ARGS__) const = 0;
        #include "interface.h"
        #undef SIMD_REGISTER
    };

    DispatchBase* GetDispatch();
}
