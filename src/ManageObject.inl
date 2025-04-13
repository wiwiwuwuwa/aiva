#include "ManageObject.hpp"

#include "Console.hpp"
#include "CstrView.hpp"
#include "Ensures.hpp"
#include "New.hpp"
#include "Process.hpp"
#include "Templates.hpp"


namespace Aiva
{
    template <typename TType>
    template <typename... TArgs>
    constexpr void ManageObject<TType>::Construct(TArgs&&... args)
    {
        if (m_ptr)
            CheckNoEntry();

        new (m_object) TType{ Templates::Forward<TArgs>(args)... };
        m_ptr = (TType*)m_object;
    }


    template <typename TType>
    constexpr void ManageObject<TType>::Destruct()
    {
        if (!m_ptr)
            CheckNoEntry();

        m_ptr = {};
        reinterpret_cast<TType*>(m_object)->~TType();
    }


    template <typename TType>
    constexpr TType& ManageObject<TType>::GetObject()
    {
        if (!m_ptr)
            CheckNoEntry();

        return *m_ptr;
    }


    template <typename TType>
    constexpr TType const& ManageObject<TType>::GetObject() const
    {
        if (!m_ptr)
            CheckNoEntry();

        return *m_ptr;
    }


    template <typename TType>
    constexpr ManageObject<TType>::operator bool() const
    {
        return m_ptr;
    }


    template <typename TType>
    constexpr TType& ManageObject<TType>::operator*()
    {
        if (!m_ptr)
            CheckNoEntry();

        return *m_ptr;
    }


    template <typename TType>
    constexpr TType const& ManageObject<TType>::operator*() const
    {
        if (!m_ptr)
            CheckNoEntry();

        return *m_ptr;
    }


    template <typename TType>
    constexpr TType* ManageObject<TType>::operator->()
    {
        if (!m_ptr)
            CheckNoEntry();

        return m_ptr;
    }


    template <typename TType>
    constexpr TType const* ManageObject<TType>::operator->() const
    {
        if (!m_ptr)
            CheckNoEntry();

        return m_ptr;
    }
}
