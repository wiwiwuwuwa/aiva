#pragma once
#include "IntrusiveRef.hpp"
#include "AllocatorBase.hpp"
#include "Ensures.hpp"
#include "Memory.hpp"
#include "Templates.hpp"


namespace Aiva
{
    template <typename TObject>
    template <typename... TArgs>
    IntrusiveRef<TObject>::IntrusiveRef(TArgs&&... args)
    {
        m_objectHolder = &Memory::GetHeapAlloc().Create<ObjectHolder_t>(Forward(args)...);
        m_objectHolder->IncRef();
    }


    template <typename TObject>
    IntrusiveRef<TObject>::IntrusiveRef(IntrusiveRef const& other)
    {
        if (this == &other)
            return;

        *this = other;
    }


    template <typename TObject>
    IntrusiveRef<TObject>& IntrusiveRef<TObject>::operator=(IntrusiveRef const& other)
    {
        if (!other.m_objectHolder || !other.m_objectHolder->HasRef())
            CheckNoEntry();

        if (this == &other)
            return *this;

        this->~IntrusiveRef();

        m_objectHolder = other.m_objectHolder;
        m_objectHolder->IncRef();

        return *this;
    }


    template <typename TObject>
    IntrusiveRef<TObject>::IntrusiveRef(IntrusiveRef&& other)
    {
        if (this == &other)
            return;

        *this = Move(other);
    }


    template <typename TObject>
    IntrusiveRef<TObject>& IntrusiveRef<TObject>::operator=(IntrusiveRef&& other)
    {
        if (!other.m_objectHolder || !other.m_objectHolder->HasRef())
            CheckNoEntry();

        if (this == &other)
            return *this;

        this->~IntrusiveRef();

        m_objectHolder = other.m_objectHolder;
        other.m_objectHolder = nullptr;

        return *this;
    }



    template <typename TObject>
    IntrusiveRef<TObject>::~IntrusiveRef()
    {
        if (!m_objectHolder)
            return;

        if (!m_objectHolder->HasRef())
            CheckNoEntry();

        m_objectHolder->DecRef();

        if (m_objectHolder->HasRef())
            return;

        Memory::GetHeapAlloc().Delete(m_objectHolder);
    }


    template <typename TObject>
    TObject* IntrusiveRef<TObject>::operator->() const
    {
        if (!m_objectHolder || !m_objectHolder->HasRef())
            CheckNoEntry();

        return &m_objectHolder->GetObject();
    }
}
// namespace Aiva
