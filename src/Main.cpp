#include "Console.hpp"
#include "Coroutines.hpp"
#include "Memory.hpp"
#include "Process.hpp"


static void TEST_COROUTINE_A(Aiva::uintptr_t const)
{
    for (auto i = Aiva::size_t{ 1 }; i <= Aiva::size_t{ 5 }; i++)
    {
        Aiva::Console::Print(" a ");
        Aiva::Coroutines::Yield();
        Aiva::Console::Print(" b ");
        Aiva::Coroutines::Yield();
        Aiva::Console::Print(" c ");
        Aiva::Coroutines::Yield();
    }
    Aiva::Coroutines::Close();
}


static void TEST_COROUTINE_B(Aiva::uintptr_t const)
{
    for (auto i = Aiva::size_t{ 1 }; i <= Aiva::size_t{ 5 }; i++)
    {
        Aiva::Console::Print(" 1 ");
        Aiva::Coroutines::Yield();
        Aiva::Console::Print(" 2 ");
        Aiva::Coroutines::Yield();
        Aiva::Console::Print(" 3 ");
        Aiva::Coroutines::Yield();
    }
    Aiva::Coroutines::Close();
}


void Main()
{
    Aiva::Console::InitSystem();
    Aiva::Memory::InitSystem();
    Aiva::Coroutines::InitSystem();

    Aiva::Console::PrintLine("Hello World!");
    Aiva::Coroutines::Spawn(TEST_COROUTINE_A, 0);
    Aiva::Coroutines::Spawn(TEST_COROUTINE_B, 0);
    while (true) {};

    Aiva::Coroutines::ShutSystem();
    Aiva::Memory::ShutSystem();
    Aiva::Console::ShutSystem();

    Aiva::Process::ExitSuccess();
}
