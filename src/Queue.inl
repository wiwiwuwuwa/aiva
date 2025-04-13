#pragma once
#include "Queue.hpp"

#include "Allocators.hpp"
#include "Ensures.hpp"
#include "Memory.hpp"
#include "Templates.hpp"


namespace Aiva
{
    template <typename TType>
    Queue<TType>::~Queue()
    {
        auto node = m_head;

        while (node)
        {
            auto const next = node->m_next;
            Memory::GetHeapAlloc().Delete(*node);

            node = next;
        }
    }


    template <typename TType>
    bool Queue<TType>::IsEmpty() const
    {
        return !m_head;
    }


    template <typename TType>
    void Queue<TType>::PushBack(TType const& data)
    {
        auto const node = &Memory::GetHeapAlloc().Create<Node>(data);

        if (!m_head)
        {
            m_head = node;
            m_tail = node;
        }
        else
        {
            m_tail->m_next = node;
            m_tail = node;
        }
    }


    template <typename TType>
    void Queue<TType>::PushBack(TType&& data)
    {
        auto const node = &Memory::GetHeapAlloc().Create<Node>(Templates::Move(data));

        if (!m_head)
        {
            m_head = node;
            m_tail = node;
        }
        else
        {
            m_tail->m_next = node;
            m_tail = node;
        }
    }


    template <typename TType>
    TType Queue<TType>::PopFront()
    {
        if (!m_head)
            CheckNoEntry();

        auto const data = Templates::Move(m_head->m_data);

        if (m_head == m_tail)
        {
            Memory::GetHeapAlloc().Delete(*m_head);

            m_head = {};
            m_tail = {};
        }
        else
        {
            auto const head = m_head->m_next;
            Memory::GetHeapAlloc().Delete(*m_head);

            m_head = head;
        }

        return data;
    }
}
// namespace Aiva
