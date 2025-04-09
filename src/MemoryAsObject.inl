#include "MemoryAsObject.hpp"
#include "New.hpp"
#include "Templates.hpp"


namespace Aiva
{
    template <typename TType>
    template <typename... TArgs>
    constexpr void MemoryAsObject<TType>::Construct(TArgs&&... args)
    {
        new (m_object) TType{ Forward<TArgs>(args)... };
    }


    template <typename TType>
    constexpr void MemoryAsObject<TType>::Destruct()
    {
        GetObject().~TType();
    }


    template <typename TType>
    constexpr TType& MemoryAsObject<TType>::GetObject()
    {
        return reinterpret_cast<TType&>(m_object);
    }


    template <typename TType>
    constexpr TType const& MemoryAsObject<TType>::GetObject() const
    {
        return reinterpret_cast<TType const&>(m_object);
    }
}
