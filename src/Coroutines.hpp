#pragma once
#include "Numbers.hpp"


namespace Aiva::Coroutines
{
    using CoroutineAction_t = void(&)(uintptr_t const /*userData*/);


    void InitSystem();
    void WaitSystem();
    void ShutSystem();

    void Start(CoroutineAction_t coroutineAction, uintptr_t const userData);
    void Yield();
}
// namespace Aiva::Coroutines
