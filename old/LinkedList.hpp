#pragma once

#include "NonCopyable.hpp"


namespace Aiva
{
    template <typename TType>
    class LinkedList final : public NonCopyable
    {
    public:
        LinkedList() = default;
        ~LinkedList();

        bool IsEmpty() const;
        void PushBack(TType const& data);
        void PushBack(TType&& data);
        TType PopFront();

    public:
        template <typename TCond>
        TType PopFirst(TCond const& condition);

    private:
        struct Node final
        {
            TType m_data;
            Node* m_next{};
        };

        Node* m_head{};
        Node* m_tail{};
    };
}
// namespace Aiva


#include "LinkedList.inl"
