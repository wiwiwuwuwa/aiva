#include <WinApi.hpp>

using namespace Aiva;
using namespace MinLib;
using namespace WinApi;

extern "C" void Main()
{
    constexpr auto const msg = StaticString<32>{"Hello World!\n"};
    void *hStdOut = GetStdHandle(-11);
    uint32_t written;
    WriteFile(hStdOut, msg, msg.GetLength(), &written, nullptr);
    ExitProcess(0);
}
