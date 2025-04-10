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
        auto const spanOfBytes = Alloc(sizeof(TType));
        if (!spanOfBytes)
            CheckNoEntry();

        auto const spanOfObject = CastSpan<TType>(spanOfBytes);
        if (!spanOfObject)
            CheckNoEntry();

        auto& object = spanOfObject.GetData();
        new (&object) TType{ Forward<TArgs>(args)... };

        return object;
    }


    template <typename TType, typename... TArgs>
    Span<TType> AllocatorBase::CreateArray(size_t const size, TArgs&&... args) const
    {
        if (size <= 0)
            CheckNoEntry();

        auto const spanOfBytes = Alloc(sizeof(TType) * size);
        if (!spanOfBytes)
            CheckNoEntry();

        auto const spanOfObjects = CastSpan<TType>(spanOfBytes);
        if (!spanOfObjects)
            CheckNoEntry();

        for (auto i = size_t{}; i < spanOfObjects.GetSize(); i++)
        {
            auto& object = spanOfObjects[i];
            new (&object) TType{ Forward<TArgs>(args)... };
        }

        return spanOfObjects;
    }


    template <typename TType>
    decltype(nullptr) AllocatorBase::Delete(TType& data) const
    {
        auto const spanOfObject = Span{ data };
        if (!spanOfObject)
            CheckNoEntry();

        auto const spanOfBytes = CastSpan<byte_t>(spanOfObject);
        if (!spanOfBytes)
            CheckNoEntry();

        data.~TType();

        return Free(spanOfBytes);
    }


    template <typename TType>
    decltype(nullptr) AllocatorBase::DeleteArray(Span<TType> const& data) const
    {
        if (!data)
            CheckNoEntry();

        auto const spanOfBytes = CastSpan<byte_t>(data);
        if (!spanOfBytes)
            CheckNoEntry();

        for (auto i = data.GetSize(); i > size_t{}; i--)
        {
            auto& object = data[i - 1];
            object.~TType();
        }

        return Free(spanOfBytes);
    }
}
// namespace Aiva
