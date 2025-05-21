#include "Console.hpp"
#include "Coroutines.hpp"
#include "Memory.hpp"
#include "Windows.hpp"


void Main()
{
    Aiva::Console::InitSystem();
    Aiva::Memory::InitSystem();
    Aiva::Coroutines::InitSystem();
    Aiva::Windows::InitSystem();

    Aiva::Console::PrintLine("Hello World!");
    while (true) {};

    Aiva::Windows::ShutSystem();
    Aiva::Coroutines::ShutSystem();
    Aiva::Memory::ShutSystem();
    Aiva::Console::ShutSystem();

    Aiva::Process::ExitSuccess();
}
