#pragma once


namespace Aiva
{
    class IAllocator;
}
// namespace Aiva


namespace Aiva::Memory
{
    void InitSystem();
    void ShutSystem();

    IAllocator const& GetHeapAlloc();
}
// namespace Aiva::Memory
