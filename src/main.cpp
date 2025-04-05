#include "console.hpp"
#include "memory.hpp"
#include "process.hpp"


void Main()
{
    aiva::Memory::InitSystem();
    aiva::Console::PrintLine("Hello World!");
    aiva::Memory::ShutSystem();
    aiva::Process::ExitSuccess();
}
