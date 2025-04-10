#pragma once
#include "Numbers.hpp"


namespace Aiva
{
    class AllocatorBase;
}
// namespace Aiva


namespace Aiva::Memory
{
    void InitSystem();
    void ShutSystem();

    AllocatorBase const& GetHeapAlloc();
}
// namespace Aiva::Memory
