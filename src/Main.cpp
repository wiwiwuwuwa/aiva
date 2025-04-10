#include "Console.hpp"
#include "Memory.hpp"
#include "Process.hpp"


void Main()
{
    Aiva::Memory::InitSystem();
    Aiva::Console::InitSystem();

    Aiva::Console::PrintLine("Hello World!");

    Aiva::Console::ShutSystem();
    Aiva::Memory::ShutSystem();

    Aiva::Process::ExitSuccess();
}
