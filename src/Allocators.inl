#pragma once

#include "Allocators.hpp"

#include "Ensures.hpp"
#include "New.hpp"
#include "Templates.hpp"


namespace Aiva
{
    template <typename TType>
    Span<TType> AllocatorBase::AllocArray(size_t const size) const
    {
        auto const spanOfBytes = Alloc(sizeof(TType) * size);
        if (!spanOfBytes)
            CheckNoEntry();

        auto const spanOfObjects = CastSpan<TType>(spanOfBytes);
        if (!spanOfObjects)
            CheckNoEntry();

        return spanOfObjects;
    }


    template <typename TType>
    nullptr_t AllocatorBase::FreeArray(Span<TType> const& span) const
    {
        auto const spanOfObjects = span;
        if (!spanOfObjects)
            CheckNoEntry();

        auto const spanOfBytes = CastSpan<byte_t>(spanOfObjects);
        if (!spanOfBytes)
            CheckNoEntry();

        return Free(spanOfBytes);
    }


    template <typename TType, typename... TArgs>
    TType& AllocatorBase::Create(TArgs&&... args) const
    {
        auto const spanOfBytes = Alloc(sizeof(TType));
        auto const spanOfObject = CastSpan<TType>(spanOfBytes);

        auto& object = spanOfObject.GetDataRef();
        new (&object) TType{ Templates::Forward<TArgs>(args)... };

        return object;
    }


    template <typename TType, typename... TArgs>
    Span<TType> AllocatorBase::CreateArray(size_t const size, TArgs&&... args) const
    {
        auto const spanOfBytes = Alloc(sizeof(TType) * size);
        auto const spanOfObjects = CastSpan<TType>(spanOfBytes);

        for (auto i = size_t{}; i < spanOfObjects.GetSize(); i++)
        {
            auto& object = spanOfObjects[i];
            new (&object) TType{ Templates::Forward<TArgs>(args)... };
        }

        return spanOfObjects;
    }


    template <typename TType>
    nullptr_t AllocatorBase::Delete(TType& data) const
    {
        auto const spanOfObject = Span{ data };
        auto const spanOfBytes = CastSpan<byte_t>(spanOfObject);

        data.~TType();
        return Free(spanOfBytes);
    }


    template <typename TType>
    nullptr_t AllocatorBase::DeleteArray(Span<TType> const& data) const
    {
        for (auto i = data.GetSize(); i > size_t{}; i--)
        {
            auto& object = data[i - 1];
            object.~TType();
        }

        auto const spanOfBytes = CastSpan<byte_t>(data);
        return Free(spanOfBytes);
    }
}
// namespace Aiva
