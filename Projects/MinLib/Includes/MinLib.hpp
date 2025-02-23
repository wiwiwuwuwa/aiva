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

    class alignas(4) uint32_t final
    {
    public:
        constexpr uint32_t();

    private:
        byte_t m_data[4]{};
    };

    static_assert(sizeof(uint32_t) == 4);
    static_assert(alignof(uint32_t) == 4);

    class alignas(4) int32_t final
    {
    public:
        constexpr int32_t();

    private:
        byte_t m_data[4]{};
    };

    constexpr byte_t::byte_t() = default;

    constexpr uint32_t::uint32_t() = default;

    constexpr int32_t::int32_t() = default;
}
// namespace Aiva::MinLib
