#include "Console.hpp"
#include "Coroutines.hpp"
#include "Memory.hpp"
#include "Process.hpp"


void CoroutineA(Aiva::uintptr_t const)
{
    Aiva::Console::Print(" da ");
    Aiva::Coroutines::Yield();
    Aiva::Console::Print(" da ");
    Aiva::Coroutines::Yield();
    Aiva::Console::Print(" da ");
    Aiva::Coroutines::Close();
}


void CoroutineB(Aiva::uintptr_t const)
{
    Aiva::Console::Print(" ne ");
    Aiva::Coroutines::Yield();
    Aiva::Console::Print(" ne ");
    Aiva::Coroutines::Yield();
    Aiva::Console::Print(" ne ");
    Aiva::Coroutines::Close();
}


void Main()
{
    Aiva::Console::InitSystem();
    Aiva::Memory::InitSystem();
    Aiva::Coroutines::InitSystem();

    Aiva::Console::PrintLine("Hello World!");
    Aiva::Coroutines::Spawn(CoroutineA);
    Aiva::Coroutines::Spawn(CoroutineB);
    while (true) {};

    Aiva::Coroutines::ShutSystem();
    Aiva::Memory::ShutSystem();
    Aiva::Console::ShutSystem();

    Aiva::Process::ExitSuccess();
}
