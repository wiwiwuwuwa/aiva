#pragma once

namespace Aiva::MinLib
{
    template <typename t_from, typename t_to>
    constexpr t_to union_cast(t_from const&const from) noexcept
    {
        static_assert(sizeof(t_from) == sizeof(t_to));

        union
        {
            t_from m_from;
            t_to m_to;
        };

        m_from = from;
        return m_to;
    }
}
