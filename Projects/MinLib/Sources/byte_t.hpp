#pragma once

namespace Aiva::MinLib
{
    class alignas(1) byte_t final
    {
    public:
        constexpr byte_t();

    private:
        unsigned char m_data{};
    };

    static_assert(sizeof(byte_t) == 1);
    static_assert(alignof(byte_t) == 1);

    // ----------------------------------------------------

    constexpr byte_t::byte_t() = default;
}
