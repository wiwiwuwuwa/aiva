#pragma once

namespace Aiva::MinLib
{
    class alignas(4) int32_t final
    {
    public:
        constexpr int32_t();

    private:
        byte_t m_data[4]{};
    };

    static_assert(sizeof(int32_t) == 4);
    static_assert(alignof(int32_t) == 4);

    // ----------------------------------------------------

    constexpr int32_t::int32_t() = default;
}
