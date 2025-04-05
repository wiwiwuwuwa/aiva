#include "console.hpp"
#include "memory.hpp"
#include "process.hpp"


void Main()
{
    aiva::Console::InitSystem();
    aiva::Memory::InitSystem();

    aiva::Console::PrintLine("Hello World!");

    aiva::Memory::ShutSystem();
    aiva::Console::ShutSystem();

    aiva::Process::ExitSuccess();
}
