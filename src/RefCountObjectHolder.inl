#pragma once
#include "RefCountObjectHolder.hpp"
#include "Ensures.hpp"
#include "Templates.hpp"


namespace Aiva
{
    template <typename TObject>
    template <typename... TArgs>
    RefCountObjectHolder<TObject>::RefCountObjectHolder(TArgs&&... args) : m_object{ Forward(args)... } {}


    template <typename TObject>
    TObject& RefCountObjectHolder<TObject>::GetObject()
    {
        if (!HasRef())
            CheckNoEntry();

        return m_object;
    }


    template <typename TObject>
    TObject const& RefCountObjectHolder<TObject>::GetObject() const
    {
        if (!HasRef())
            CheckNoEntry();

        return m_object;
    }
}
// namespace Aiva
