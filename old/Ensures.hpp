#pragma once

#include "Console.hpp"
#include "CstrView.hpp"
#include "Process.hpp"


#define CheckNoEntry_Stringify(x) #x
#define CheckNoEntry_ToString(x) CheckNoEntry_Stringify(x)
#define CheckNoEntry_GetCodeLine() CheckNoEntry_ToString(__LINE__)


#define CheckNoEntry() \
{ \
    Aiva::Console::ErrorLine(__FILE__, "(", CheckNoEntry_GetCodeLine(), "): ", __func__, ": NO_ENTRY_POINT."); \
    Aiva::Process::ExitFailure(); \
} \
