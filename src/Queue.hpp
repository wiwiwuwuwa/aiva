#pragma once


namespace Aiva
{
    template <typename TType>
    class Queue final
    {
    public:
        Queue() = default;
        ~Queue();

        Queue(const Queue&) = delete;
        Queue& operator=(const Queue&) = delete;
        Queue(Queue&&) = delete;
        Queue& operator=(Queue&&) = delete;

        bool IsEmpty() const;
        void PushBack(TType const& data);
        void PushBack(TType&& data);
        TType PopFront();

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


#include "Queue.inl"
