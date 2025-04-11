#pragma once


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <intrin.h>
#pragma clang diagnostic pop


#ifdef __INTELLISENSE__
    extern "C" long __stdcall InterlockedCompareExchange(volatile long *const Destination, long const Exchange, long const Comperand);
    extern "C" long __stdcall InterlockedExchange(volatile long *const Destination, long const Exchange);
#else
    #define InterlockedCompareExchange _InterlockedCompareExchange
    #define InterlockedExchange _InterlockedExchange
#endif // __INTELLISENSE__
