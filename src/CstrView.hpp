#pragma once

#include "Numbers.hpp"


namespace Aiva
{
    class CstrView final
    {
    public:
        CstrView();
        CstrView(char const*const data);
        operator bool() const;
        operator char const*() const;
        char const* c_str() const;
        size_t StrLen() const;
        char operator[](size_t const index) const;

    private:
        char const& m_data;
    };


    size_t StrLen(CstrView const str);
}
// namespace Aiva
