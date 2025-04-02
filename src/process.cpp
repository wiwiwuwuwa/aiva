#include "process.hpp"
#include "winapi.hpp"

namespace aiva
{
    void ExitProcess(uintptr_t const code)
    {
        winapi::ExitProcess((uint32_t)code);
    }
}
// namespace aiva
