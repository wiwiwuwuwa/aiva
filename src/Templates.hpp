#pragma once


namespace Aiva::Templates
{
    // ========================================================================
    // Basic
    // ========================================================================

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
    constexpr TType&& Forward(TType& value);


    template <typename TType>
    constexpr TType&& Forward(TType&& value);


    template <typename TType>
    constexpr RemoveReference_t<TType>&& Move(TType&& value);

    // ========================================================================
    // Numbers
    // ========================================================================

    enum class NumberType
    {
        Int,
        Flt,
        MAX,
    };


    enum class NumberSign
    {
        None,
        Sign,
        MAX,
    };


    enum class NumberSize
    {
        Byte1,
        Byte2,
        Byte4,
        Byte8,
        MAX,
    };


    template <typename TType>
    constexpr bool IsInteger();


    template <typename TType>
    constexpr bool IsFloating();


    template <typename TType>
    constexpr bool IsUnsigned();


    template <typename TType>
    constexpr bool IsSigned();


    template <typename TType>
    constexpr NumberType GetNumberType();


    template <typename TType>
    constexpr NumberSign GetNumberSign();


    template <typename TType>
    constexpr NumberSize GetNumberSize();


    template <NumberType TType, NumberSign TSign, NumberSize TSize>
    constexpr auto CreateNumber();


    template <NumberType TType, NumberSign TSign, NumberSize TSize>
    struct NumberIdentity final : public TypeIdentity<decltype(CreateNumber<TType, TSign, TSize>())> {};

    template <NumberType TType, NumberSign TSign, NumberSize TSize>
    using NumberIdentity_t = typename NumberIdentity<TType, TSign, TSize>::Type_t;
}
// namespace Aiva::Templates


#include "Templates.inl"
