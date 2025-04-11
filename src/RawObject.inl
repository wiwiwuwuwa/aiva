#include "RawObject.hpp"
#include "New.hpp"
#include "Templates.hpp"


namespace Aiva
{
    template <typename TType>
    template <typename... TArgs>
    constexpr void RawObject<TType>::Construct(TArgs&&... args)
    {
        new (m_object) TType{ Templates::Forward<TArgs>(args)... };
    }


    template <typename TType>
    constexpr void RawObject<TType>::Destruct()
    {
        GetObject().~TType();
    }


    template <typename TType>
    constexpr TType& RawObject<TType>::GetObject()
    {
        return reinterpret_cast<TType&>(m_object);
    }


    template <typename TType>
    constexpr TType const& RawObject<TType>::GetObject() const
    {
        return reinterpret_cast<TType const&>(m_object);
    }
}
