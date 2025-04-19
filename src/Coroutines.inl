#pragma once

#include "Allocators.hpp"
#include "Coroutines.hpp"
#include "Ensures.hpp"
#include "Memory.hpp"
#include "Templates.hpp"


namespace Aiva::Coroutines
{
    template <typename TCoroutine>
    void Spawn(TCoroutine&& coroutine, uintptr_t const workerMask)
    {
        class CoroutineWrapper final
        {
        public:
            static void CoroutineFunc(Core::Control const& control)
            {
                auto const coroutineInstance = (TCoroutine*)control.GetUserData();
                if (!coroutineInstance)
                    CheckNoEntry();

                class ControlWrapper final : public IControl
                {
                public:
                    ControlWrapper(Core::Control const& control) : m_control{ control } {}
                    uintptr_t GetCurrWorker() const override { return m_control.GetCurrWorker(); }
                    void Yield(uintptr_t const workerMask) const override { m_control.YieldOnWorker(workerMask); }

                private:
                    Core::Control const& m_control;
                };

                auto const controlInstance = ControlWrapper{ control };
                (*coroutineInstance)(controlInstance);

                Memory::GetHeapAlloc().Delete(*coroutineInstance);
            }

        private:
            CoroutineWrapper() = delete;
        };

        auto& coroutineInstance = Memory::GetHeapAlloc().Create<TCoroutine>(Templates::Forward<TCoroutine>(coroutine));
        Core::SpawnOnWorker(CoroutineWrapper::CoroutineFunc, workerMask, (uintptr_t)&coroutineInstance);
    }
}
