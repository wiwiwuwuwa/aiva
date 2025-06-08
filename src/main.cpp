#include "aiva.hpp"


void Main()
{
    Aiva::Console::InitSystem();
    Aiva::Memory::InitSystem();
    Aiva::Coroutines::InitSystem();
    Aiva::Windows::InitSystem();

    while (true) {}

    Aiva::Windows::ShutSystem();
    Aiva::Coroutines::ShutSystem();
    Aiva::Memory::ShutSystem();
    Aiva::Console::ShutSystem();

    Aiva::System::ExitSuccess();
}
