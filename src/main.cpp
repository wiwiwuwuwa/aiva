#include "aiva.hpp"


void main()
{
    Aiva::Console::InitSystem();
    Aiva::Console::PrintLine("hello world");
    Aiva::Console::ShutSystem();

    while (true) {}
}
