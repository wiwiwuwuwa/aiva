#pragma once
#include "Numbers.hpp"


namespace Aiva::Threads
{
    using ThreadAction_t = void(&)(uintptr_t const /*userData*/);


    size_t GetNumberOfCores();
    void CreateThread(ThreadAction_t threadAction, uintptr_t const userData);
}
// namespace Aiva::Threads
