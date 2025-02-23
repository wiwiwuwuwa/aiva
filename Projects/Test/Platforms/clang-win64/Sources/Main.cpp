#include <WinApi.hpp>

using namespace Aiva;
using namespace MinLib;
using namespace WinApi;

extern "C" void Main()
{
    const char msg[] = "Hello World!\n";

    const unsigned int flgA = (unsigned int) -11;
    const uint32_t flgB = *reinterpret_cast<uint32_t const*>(&flgA);
    void *hStdOut = GetStdHandle(flgB);

    const unsigned int szeA = sizeof(msg) - 1;
    const uint32_t szeB = *reinterpret_cast<uint32_t const*>(&szeA);
    uint32_t written;
    WriteFile(hStdOut, msg, szeB, &written, 0);

    const unsigned int extA = 0;
    const uint32_t extB = *reinterpret_cast<uint32_t const*>(&extA);
    ExitProcess(extB);
}
