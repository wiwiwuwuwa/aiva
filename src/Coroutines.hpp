#pragma once

#include "Coroutines_Minimal.hpp"


namespace Aiva::Coroutines
{
    auto constexpr kAnyWorkerMask = Minimal::kAnyWorkerMask;


    class IControl
    {
    public:
        virtual uintptr_t GetCurrWorker() const = 0;
        virtual void Yield(uintptr_t const workerMask = kAnyWorkerMask) const = 0;

        virtual ~IControl() = default;

    public:
        void YieldOnCurrWorker() const;
    };


    void InitSystem();
    void ShutSystem();


    template <typename TCoroutine>
    void Spawn(TCoroutine&& coroutine, uintptr_t const workerMask = kAnyWorkerMask);
}
// namespace Aiva::Coroutines


#include "Coroutines.inl"
