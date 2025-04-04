#include "console.hpp"
#include "memory.hpp"
#include "process.hpp"

void Main()
{
    aiva::InitMemorySystem();
    aiva::PrintLine("Hello World!");
    aiva::ShutMemorySystem();
    aiva::ExitProcess();
}
