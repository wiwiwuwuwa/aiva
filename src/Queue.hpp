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
