#pragma once

namespace Aiva::MinLib
{
    class alignas(4) uint32_t final
    {
    public:
        constexpr uint32_t();

    private:
        byte_t m_data[4]{};
    };

    static_assert(sizeof(uint32_t) == 4);
    static_assert(alignof(uint32_t) == 4);

    // ----------------------------------------------------

    constexpr uint32_t::uint32_t() = default;
}
