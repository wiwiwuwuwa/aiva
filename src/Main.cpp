#include "Console.hpp"
#include "Coroutines.hpp"
#include "Memory.hpp"
#include "Process.hpp"


static void TEST_COROUTINE(Aiva::uintptr_t const)
{
    Aiva::Console::Print(" a ");
    Aiva::Coroutines::Yield();
    Aiva::Console::Print(" b ");
    Aiva::Coroutines::Yield();
    Aiva::Console::Print(" c ");
    Aiva::Coroutines::Close();
}


void Main()
{
    Aiva::Console::InitSystem();
    Aiva::Memory::InitSystem();
    Aiva::Coroutines::InitSystem();

    Aiva::Console::PrintLine("Hello World!");
    Aiva::Coroutines::Spawn(TEST_COROUTINE, 0);
    while (true) {};

    Aiva::Coroutines::ShutSystem();
    Aiva::Memory::ShutSystem();
    Aiva::Console::ShutSystem();

    Aiva::Process::ExitSuccess();
}
