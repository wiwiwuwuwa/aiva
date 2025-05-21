#include "Coroutines.hpp"


using namespace Aiva;
using namespace Aiva::Coroutines;


void IControl::YieldOnCurrWorker() const
{
    Yield(GetCurrWorker());
}


void Coroutines::InitSystem()
{
    Minimal::InitSystem();
}


void Coroutines::ShutSystem()
{
    Minimal::ShutSystem();
}
