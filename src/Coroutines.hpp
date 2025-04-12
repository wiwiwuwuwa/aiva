#pragma once
#include "Numbers.hpp"


namespace Aiva::Coroutines
{
    using CoroutineAction_t = void(&)(uintptr_t const /*userData*/);


    void InitSystem();
    void ShutSystem();

    void Spawn(CoroutineAction_t coroutineAction, uintptr_t const userData = {});
    void Yield();
    void Close();
}
// namespace Aiva::Coroutines
