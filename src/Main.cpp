#include "Console.hpp"
#include "Coroutines.hpp"
#include "Memory.hpp"
#include "Process.hpp"


void Main()
{
    Aiva::Console::InitSystem();
    Aiva::Memory::InitSystem();
    Aiva::Coroutines::InitSystem();

    Aiva::Console::PrintLine("Hello World!");
    Aiva::Coroutines::WaitSystem();

    Aiva::Coroutines::ShutSystem();
    Aiva::Memory::ShutSystem();
    Aiva::Console::ShutSystem();

    Aiva::Process::ExitSuccess();
}
