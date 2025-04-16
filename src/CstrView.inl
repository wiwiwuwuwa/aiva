#pragma once
#include "CstrView.hpp"


namespace Aiva
{
    constexpr CstrView::CstrView() : m_data{ *"" }
    {

    }


    constexpr CstrView::CstrView(char const*const data) : m_data{ data ? *data : *"" }
    {

    }


    constexpr CstrView::operator bool() const
    {
        return (&m_data)[0] != '\0';
    }


    constexpr CstrView::operator char const*() const
    {
        return &m_data;
    }


    constexpr char const* CstrView::c_str() const
    {
        return &m_data;
    }


    constexpr size_t CstrView::StrLen() const
    {
        auto length = size_t{};

        while ((&m_data)[length] != '\0')
            length++;

        return length;
    }


    constexpr char CstrView::operator[](size_t const index) const
    {
        return (&m_data)[index];
    }


    constexpr size_t StrLen(CstrView const str)
    {
        return str.StrLen();
    }
}
// namespace Aiva
