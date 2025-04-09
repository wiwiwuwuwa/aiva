#pragma once
#include "Console.hpp"
#include "Process.hpp"


#define CheckNoEntry() \
{ \
    Aiva::Process::ExitFailure(); \
} \


#define CheckNoEntryMsg(msg) \
{ \
    if (msg) \
    { \
        Aiva::Console::Error(__FILE__); \
        Aiva::Console::Error(": "); \
        Aiva::Console::Error(__func__); \
        Aiva::Console::Error(": "); \
        Aiva::Console::Error(msg); \
        Aiva::Console::Error(".\n"); \
    } \
    CheckNoEntry(); \
} \
