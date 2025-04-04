#pragma once


namespace aiva
{
    template <typename TType>
    struct TypeIdentity
    {
        using Type_t = TType;
    };

    template <typename TType>
    using TypeIdentity_t = typename TypeIdentity<TType>::Type_t;

    template <typename TType>
    struct RemoveReference : public TypeIdentity<TType> {};

    template <typename TType>
    struct RemoveReference<TType&> : public TypeIdentity<TType> {};

    template <typename TType>
    struct RemoveReference<TType&&> : public TypeIdentity<TType> {};

    template <typename TType>
    using RemoveReference_t = typename RemoveReference<TType>::Type_t;

    template <typename TType>
    TType&& Forward(TType& value) { return static_cast<TType&&>(value); }

    template <typename TType>
    TType&& Forward(TType&& value) { return static_cast<TType&&>(value); }
}
// namespace aiva
