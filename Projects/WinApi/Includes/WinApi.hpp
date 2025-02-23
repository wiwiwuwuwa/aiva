#pragma once
#include <MinLib.hpp>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type-c-linkage"

extern "C" { namespace Aiva::WinApi
{
    void* __stdcall GetStdHandle(MinLib::uint32_t const);
    MinLib::int32_t __stdcall WriteFile(void*const, void const*const, MinLib::uint32_t const, MinLib::uint32_t *const, void *const);
    void __stdcall ExitProcess(MinLib::uint32_t const);
}}
// extern "C" namespace Aiva::WinApi

#pragma GCC diagnostic pop
