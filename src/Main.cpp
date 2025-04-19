#include "Console.hpp"
#include "Coroutines.hpp"
#include "Memory.hpp"
#include "Process.hpp"
#include "StaticString.hpp"


static void TEST_COROUTINE_A(Aiva::Coroutines::IControl const& control)
{
    Aiva::CstrView const messages1[] = { "(coroutine a : step 1 : ", Aiva::ToStaticString(control.GetCurrWorker()), ")" };
    Aiva::Console::PrintLine(messages1);

    control.Yield();

    Aiva::CstrView const messages2[] = { "(coroutine a : step 2 : ", Aiva::ToStaticString(control.GetCurrWorker()), ")" };
    Aiva::Console::PrintLine(messages2);

    control.Yield();

    Aiva::CstrView const messages3[] = { "(coroutine a : step 3 : ", Aiva::ToStaticString(control.GetCurrWorker()), ")" };
    Aiva::Console::PrintLine(messages3);
}


static void TEST_COROUTINE_B(Aiva::Coroutines::IControl const& control)
{
    Aiva::CstrView const messages1[] = { "(coroutine b : step 1 : ", Aiva::ToStaticString(control.GetCurrWorker()), ")" };
    Aiva::Console::PrintLine(messages1);

    control.Yield();

    Aiva::CstrView const messages2[] = { "(coroutine b : step 2 : ", Aiva::ToStaticString(control.GetCurrWorker()), ")" };
    Aiva::Console::PrintLine(messages2);

    control.Yield();

    Aiva::CstrView const messages3[] = { "(coroutine b : step 3 : ", Aiva::ToStaticString(control.GetCurrWorker()), ")" };
    Aiva::Console::PrintLine(messages3);
}


void Main()
{
    Aiva::Console::InitSystem();
    Aiva::Memory::InitSystem();
    Aiva::Coroutines::InitSystem();

    Aiva::Console::PrintLine("Hello World!");
    Aiva::Coroutines::Spawn([] (auto const& control) { TEST_COROUTINE_A(control); });
    Aiva::Coroutines::Spawn([] (auto const& control) { TEST_COROUTINE_B(control); });
    while (true) {};

    Aiva::Coroutines::ShutSystem();
    Aiva::Memory::ShutSystem();
    Aiva::Console::ShutSystem();

    Aiva::Process::ExitSuccess();
}
