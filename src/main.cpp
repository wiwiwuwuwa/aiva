#include "aiva.hpp"


void Main()
{
    Aiva::Console::InitSystem();
    Aiva::Memory::InitSystem();
    Aiva::Coroutines::InitSystem();
    Aiva::Console::PrintLine("Hello, world!");
    Aiva::Coroutines::ShutSystem();
    Aiva::Memory::ShutSystem();
    Aiva::Console::ShutSystem();

    Aiva::Process::ExitSuccess();
}
