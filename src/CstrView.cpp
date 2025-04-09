#include "CstrView.hpp"


namespace Aiva
{
    CstrView::CstrView() : m_data{ *"" }
    {

    }


    CstrView::CstrView(char const*const data) : m_data{ data ? *data : *"" }
    {

    }


    CstrView::operator bool() const
    {
        return (&m_data)[0] != '\0';
    }


    CstrView::operator char const*() const
    {
        return &m_data;
    }


    char const* CstrView::c_str() const
    {
        return &m_data;
    }


    size_t CstrView::StrLen() const
    {
        auto length = size_t{};

        while ((&m_data)[length] != '\0')
            length++;

        return length;
    }


    char CstrView::operator[](size_t const index) const
    {
        return (&m_data)[index];
    }


    size_t StrLen(CstrView const str)
    {
        return str.StrLen();
    }
}
// namespace Aiva
