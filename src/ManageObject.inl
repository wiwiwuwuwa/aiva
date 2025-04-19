#include "ManageObject.hpp"

#include "New.hpp"
#include "Templates.hpp"


namespace Aiva
{
    template <typename TType>
    template <typename... TArgs>
    constexpr void ManageObject<TType>::Construct(TArgs&&... args)
    {
        new (m_object) TType{ Templates::Forward<TArgs>(args)... };
        m_ptr = (TType*)m_object;
    }


    template <typename TType>
    constexpr void ManageObject<TType>::Destruct()
    {
        m_ptr = nullptr;
        reinterpret_cast<TType*>(m_object)->~TType();
    }


    template <typename TType>
    constexpr TType& ManageObject<TType>::GetObjectRef()
    {
        return *m_ptr;
    }


    template <typename TType>
    constexpr TType const& ManageObject<TType>::GetObjectRef() const
    {
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
        return *m_ptr;
    }


    template <typename TType>
    constexpr TType const& ManageObject<TType>::operator*() const
    {
        return *m_ptr;
    }


    template <typename TType>
    constexpr TType* ManageObject<TType>::operator->()
    {
        return m_ptr;
    }


    template <typename TType>
    constexpr TType const* ManageObject<TType>::operator->() const
    {
        return m_ptr;
    }
}
