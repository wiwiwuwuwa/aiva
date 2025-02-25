#include <WinApi.hpp>

using namespace Aiva;
using namespace MinLib;
using namespace WinApi;

extern "C" void Main()
{
    const char msg[] = "Hello World!\n";
    void *hStdOut = GetStdHandle(-11);
    uint32_t written;
    WriteFile(hStdOut, msg, sizeof(msg) - 1, &written, nullptr);
    ExitProcess(0);
}
