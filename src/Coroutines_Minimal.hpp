#pragma once

#include "Numbers.hpp"


namespace Aiva::Coroutines::Minimal
{
    struct Control;


    constexpr uintptr_t const kAnyWorkerMask = (uintptr_t)(-1);


    using GetUserDataFunc = uintptr_t(&)();
    using GetCurrWorkerFunc = uintptr_t(&)();
    using YieldFunc_t = void(&)();
    using YieldOnWorkerFunc_t = void(&)(uintptr_t const workerMask);
    using CoroutineFunc_t = void(&)(Control const& control);


    struct Control final
    {
        GetUserDataFunc GetUserData;
        GetCurrWorkerFunc GetCurrWorker;
        YieldFunc_t Yield;
        YieldOnWorkerFunc_t YieldOnWorker;
    };


    void InitSystem();
    void ShutSystem();


    void Spawn(CoroutineFunc_t coroutineFunc, uintptr_t const userData = 0);
    void SpawnOnWorker(CoroutineFunc_t coroutineFunc, uintptr_t const workerMask, uintptr_t const userData = 0);
}
// namespace Aiva::Coroutines::Core
