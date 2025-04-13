#pragma once


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
