#include "Console.hpp"
#include "Coroutines.hpp"
#include "Memory.hpp"
#include "Process.hpp"


void Coroutine(Aiva::uintptr_t const userData)
{
    auto const text = Aiva::CstrView{ (const char*)userData };

    while (true)
    {
        Aiva::Console::Print(text);
        Aiva::Coroutines::Yield();
    }
}


void Coroutines()
{
    Aiva::CstrView const texts[] =
    {
        " a ",
        " b ",
        " c ",
        " d ",
        " e ",
        " f ",
        " g ",
        " h ",
        " i ",
    };

    for (auto const text : texts)
        Aiva::Coroutines::Spawn(Coroutine, (Aiva::uintptr_t)text.c_str());
}


void Main()
{
    Aiva::Console::InitSystem();
    Aiva::Memory::InitSystem();
    Aiva::Coroutines::InitSystem();

    Aiva::Console::PrintLine("Hello World!");
    Coroutines();

    Aiva::Coroutines::ShutSystem();
    Aiva::Memory::ShutSystem();
    Aiva::Console::ShutSystem();

    Aiva::Process::ExitSuccess();
}
