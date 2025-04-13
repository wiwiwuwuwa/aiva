#pragma once


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
