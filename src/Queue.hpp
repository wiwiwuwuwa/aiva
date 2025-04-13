#pragma once

#include "NonCopyable.hpp"


namespace Aiva
{
    template <typename TType>
    class Queue final : public NonCopyable
    {
    public:
        Queue() = default;
        ~Queue();

        bool IsEmpty() const;
        void Enqueue(TType const& data);
        void Enqueue(TType&& data);
        TType Dequeue();

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
