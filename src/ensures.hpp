#pragma once
#include "console.hpp"
#include "process.hpp"


#define CheckNoEntry() \
{ \
    aiva::Process::ExitFailure(); \
} \


#define CheckNoEntryMsg(msg) \
{ \
    if (msg) \
    { \
        aiva::Console::Error(__FILE__); \
        aiva::Console::Error(": "); \
        aiva::Console::Error(__func__); \
        aiva::Console::Error(": "); \
        aiva::Console::Error(msg); \
        aiva::Console::Error(".\n"); \
    } \
    CheckNoEntry(); \
} \
