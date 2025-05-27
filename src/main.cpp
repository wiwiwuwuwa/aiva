#include "aiva.hpp"


void Main()
{
    Aiva::Console::InitSystem();
    Aiva::Console::PrintLine("Hello, world!");
    Aiva::Console::ShutSystem();

    Aiva::Process::ExitSuccess();
}
