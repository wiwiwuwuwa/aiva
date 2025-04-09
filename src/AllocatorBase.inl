#pragma once
#include "AllocatorBase.hpp"
#include "Ensures.hpp"
#include "New.hpp"
#include "Templates.hpp"


namespace Aiva
{
    template <typename TType, typename... TArgs>
    TType& AllocatorBase::Create(TArgs&&... args) const
    {
        TType& object = reinterpret_cast<TType&>(Alloc(sizeof(TType)));
        new (&object) TType{ Forward<TArgs>(args)... };

        return object;
    }


    template <typename TType, typename... TArgs>
    Span<TType> AllocatorBase::CreateArray(size_t const size, TArgs&&... args) const
    {
        if (size <= 0)
            CheckNoEntry();

        TType& objects = reinterpret_cast<TType&>(Alloc(sizeof(TType) * size));

        for (auto i = size_t{}; i < size; i++)
        {
            TType& object = (&objects)[i];
            new (&object) TType{ Forward<TArgs>(args)... };
        }

        return Span<TType>{ size, objects };
    }


    template <typename TType>
    decltype(nullptr) AllocatorBase::Delete(TType& data) const
    {
        data.~TType();
        return Free(reinterpret_cast<byte_t&>(data));
    }


    template <typename TType>
    decltype(nullptr) AllocatorBase::DeleteArray(Span<TType> const& data) const
    {
        if (!data)
            CheckNoEntry();

        for (auto i = data.GetSize(); i > size_t{}; i--)
            data[i - 1].~TType();

        return Free(reinterpret_cast<byte_t&>(data.GetData()));
    }
}
// namespace Aiva
