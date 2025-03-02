#include <SysLib.hpp>

using namespace Aiva;
using namespace SysLib;

extern "C" void Main()
{
    Console::WriteToOutput("Hello World!\n");
    Window window{ 1280, 720 };
    Ui::Loop();
    Process::Exit(ErrorCode::Success);
}
