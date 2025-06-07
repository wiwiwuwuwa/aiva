#include "aiva.hpp"


void Main()
{
    Aiva::Console::InitSystem();
    Aiva::Memory::InitSystem();
    Aiva::Coroutines::InitSystem();

    Aiva::Coroutines::Spawn([](auto const& c)
    {
        for (auto i = size_t{}; i < 5u; i++)
        {
            Aiva::Console::Print("Coroutine A\n");
            c.Yield();
        }
    });
    Aiva::Coroutines::Spawn([](auto const& c)
    {
        for (auto i = size_t{}; i < 5u; i++)
        {
            Aiva::Console::Print("Coroutine B\n");
            c.Yield();
        }
    });
    while (true) {}

    Aiva::Coroutines::ShutSystem();
    Aiva::Memory::ShutSystem();
    Aiva::Console::ShutSystem();

    Aiva::Process::ExitSuccess();
}
