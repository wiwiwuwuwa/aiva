#pragma once
#include "Numbers.hpp"


namespace Aiva
{
    class RefCountObject
    {
    public:
        void IncRef();
        void DecRef();
        bool HasRef();

    private:
        int32_t m_refs{};
    };
}
// namespace Aiva
