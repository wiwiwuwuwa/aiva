#pragma once

#include "Coroutines_Core.hpp"


namespace Aiva::Coroutines
{
    auto constexpr kAnyWorkerMask = Core::kAnyWorkerMask;


    class IControl
    {
    public:
        virtual uintptr_t GetCurrWorker() const = 0;
        virtual void Yield(uintptr_t const workerMask = kAnyWorkerMask) const = 0;

        virtual ~IControl() = default;
    };


    void InitSystem();
    void ShutSystem();


    template <typename TCoroutine>
    void Spawn(TCoroutine&& coroutine, uintptr_t const workerMask = kAnyWorkerMask);
}
// namespace Aiva::Coroutines


#include "Coroutines.inl"
