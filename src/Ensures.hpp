#pragma once
#include "Console.hpp"
#include "CstrView.hpp"
#include "Process.hpp"


#define CheckNoEntry() \
{ \
    Aiva::CstrView const messages[] = \
    { \
        __FILE__, \
        ": ", \
        __func__, \
        ": NO_ENTRY_POINT." \
    }; \
 \
    Aiva::Console::ErrorLine(messages); \
    Aiva::Process::ExitFailure(); \
} \
