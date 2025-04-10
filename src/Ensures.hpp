#pragma once
#include "Console.hpp"
#include "Process.hpp"


#define CheckNoEntry() \
{ \
    Aiva::Console::Error(__FILE__); \
    Aiva::Console::Error(": "); \
    Aiva::Console::Error(__func__); \
    Aiva::Console::Error(": Check No Entry.\n"); \
    Aiva::Process::ExitFailure(); \
} \
