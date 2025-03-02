#include <SysLib.hpp>

using namespace Aiva;
using namespace SysLib;

extern "C" void Main()
{
    Console::WriteToOutput("Hello World!\n");
    Window{ 1280, 720 }.WaitUntilClosed();
    Process::Exit(ErrorCode::Success);
}
