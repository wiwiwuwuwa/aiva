#pragma once
// です。


// ------------------------------------
// "basic_templates.hpp"


namespace Aiva::Templates
{
    template <typename...>
    using Void_t = void;
}


// ------------------------------------
// "number_templates.hpp"


namespace Aiva::Templates
{
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


    template <typename TType, typename = void>
    struct IsNumber final { constexpr operator bool() const { return false; } };


    template <typename TType>
    struct IsNumber<TType, Void_t<decltype(TType{ 3 } / TType{ 2 })>> final { constexpr operator bool() const { return true; } };


    template <typename TType>
    constexpr NumberType GetNumberType();


    template <typename TType>
    constexpr NumberSign GetNumberSign();


    template <typename TType>
    constexpr NumberSize GetNumberSize();


    template <NumberType TType, NumberSign TSign, NumberSize TSize>
    constexpr auto CreateNumber();


    template <NumberType TType, NumberSign TSign, NumberSize TSize>
    using Number_t = decltype(CreateNumber<TType, TSign, TSize>());
}


// ------------------------------------
// "number_templates.inl"


namespace Aiva::Templates
{
    template <typename TType>
    constexpr NumberType GetNumberType()
    {
        if constexpr (IsNumber<TType>())
        {
            if constexpr (TType{ 3 } / TType{ 2 } == TType{ 1 })
                { return NumberType::Int; }

            else
                { return NumberType::Flt; }
        }
        else
            { return NumberType::MAX; }
    }


    template <typename TType>
    constexpr NumberSign GetNumberSign()
    {
        if constexpr (IsNumber<TType>())
        {
            if constexpr ((TType)(-1) < (TType)(0))
                { return NumberSign::Sign; }

            else
                { return NumberSign::None; }
        }
        else
            { return NumberSign::MAX; }
    }


    template <typename TType>
    constexpr NumberSize GetNumberSize()
    {
        if constexpr (sizeof(TType) == 1)
            { return NumberSize::Byte1; }

        else if constexpr (sizeof(TType) == 2)
            { return NumberSize::Byte2; }

        else if constexpr (sizeof(TType) == 4)
            { return NumberSize::Byte4; }

        else if constexpr (sizeof(TType) == 8)
            { return NumberSize::Byte8; }

        else
            { return NumberSize::MAX; }
    }


    template <NumberType TType, NumberSign TSign, NumberSize TSize>
    constexpr auto CreateNumber()
    {
        #define CreateNumber_Begin() \
            if constexpr (TType == NumberType::MAX || TSign == NumberSign::MAX || TSize == NumberSize::MAX) \
                { return nullptr; } \

        #define CreateNumber_CheckBaseType(type) \
            else if constexpr (TType == GetNumberType<type>() && TSign == GetNumberSign<type>() && TSize == GetNumberSize<type>()) \
                { type constexpr const zero{}; return zero; } \

        #define CreateNumber_End() \
            else \
                { return nullptr; } \

        CreateNumber_Begin()
        CreateNumber_CheckBaseType(char)
        CreateNumber_CheckBaseType(signed char)
        CreateNumber_CheckBaseType(unsigned char)
        CreateNumber_CheckBaseType(short int)
        CreateNumber_CheckBaseType(unsigned short int)
        CreateNumber_CheckBaseType(int)
        CreateNumber_CheckBaseType(unsigned int)
        CreateNumber_CheckBaseType(long int)
        CreateNumber_CheckBaseType(unsigned long int)
        CreateNumber_CheckBaseType(long long int)
        CreateNumber_CheckBaseType(unsigned long long int)
        CreateNumber_CheckBaseType(float)
        CreateNumber_CheckBaseType(double)
        CreateNumber_CheckBaseType(long double)
        CreateNumber_End()

        #undef CreateNumber_End
        #undef CreateNumber_CheckBaseType
        #undef CreateNumber_Begin
    }
}


// ------------------------------------
// "primitive_types.hpp"


namespace Aiva
{
    #define DefineNumber_Classic(name, type, sign, size) \
        using name = Templates::Number_t<Templates::NumberType::type, Templates::NumberSign::sign, Templates::NumberSize::size>; \

    #define DefineNumber_Pointer(name, type, sign) \
        using name = Templates::Number_t<Templates::NumberType::type, Templates::NumberSign::sign, Templates::GetNumberSize<void*>()>; \

    DefineNumber_Classic(sint8_t, Int, Sign, Byte1)
    DefineNumber_Classic(sint16_t, Int, Sign, Byte2)
    DefineNumber_Classic(sint32_t, Int, Sign, Byte4)
    DefineNumber_Classic(sint64_t, Int, Sign, Byte8)
    DefineNumber_Classic(uint8_t, Int, None, Byte1)
    DefineNumber_Classic(uint16_t, Int, None, Byte2)
    DefineNumber_Classic(uint32_t, Int, None, Byte4)
    DefineNumber_Classic(uint64_t, Int, None, Byte8)
    DefineNumber_Classic(float32_t, Flt, None, Byte4)
    DefineNumber_Classic(float64_t, Flt, None, Byte8)
    DefineNumber_Pointer(sintptr_t, Int, Sign)
    DefineNumber_Pointer(uintptr_t, Int, None)

    #undef DefineNumber_Pointer
    #undef DefineNumber_Classic

    enum class byte_t : uint8_t {};
    using size_t = decltype(sizeof(byte_t));
    using nullptr_t = decltype(nullptr);
}


// ------------------------------------
// "winapi.hpp"


extern "C" { namespace Aiva::WinApi
{
    // Base Types

    using UINT = uint32_t;

    // Process Management

    [[noreturn]] __attribute__((dllimport, stdcall)) void ExitProcess(UINT const uExitCode);
}}


// ------------------------------------
// "process.hpp"


namespace Aiva::Process
{
    [[noreturn]] void ExitSuccess();
    [[noreturn]] void ExitFailure();
}


// ------------------------------------
// "process.inl"


namespace Aiva::Process
{
    [[noreturn]] void ExitSuccess()
    {
        WinApi::ExitProcess(0);
    }


    [[noreturn]] void ExitFailure()
    {
        WinApi::ExitProcess(1);
    }
}


// ------------------------------------
// "cstr_view.hpp"


namespace Aiva
{
    class CstrView final
    {
    public:
        constexpr CstrView();
        constexpr CstrView(char const& data);
        constexpr CstrView(char const*const data);
        constexpr operator char const&() const;
        constexpr operator char const*() const;

        constexpr operator bool() const;
        constexpr size_t StrLen() const;
        constexpr char operator[](size_t const index) const;
        constexpr char const& GetDataRef() const;
        constexpr char const* GetDataPtr() const;

    private:
        char const& m_data;
    };


    constexpr size_t StrLen(CstrView const str);
}


// ------------------------------------
// "cstr_view.inl"


namespace Aiva
{
    constexpr CstrView::CstrView() : m_data{ *"" }
    {
        //
    }


    constexpr CstrView::CstrView(char const& data) : m_data{ data }
    {
        //
    }


    constexpr CstrView::CstrView(char const*const data) : m_data{ data ? *data : *"" }
    {
        //
    }


    constexpr CstrView::operator char const&() const
    {
        return m_data;
    }


    constexpr CstrView::operator char const*() const
    {
        return &m_data;
    }


    constexpr CstrView::operator bool() const
    {
        return (&m_data)[0] != '\0';
    }


    constexpr size_t CstrView::StrLen() const
    {
        auto length = size_t{};

        while ((&m_data)[length] != '\0')
            length++;

        return length;
    }


    constexpr char CstrView::operator[](size_t const index) const
    {
        return (&m_data)[index];
    }


    constexpr size_t StrLen(CstrView const str)
    {
        return str.StrLen();
    }


    constexpr char const& CstrView::GetDataRef() const
    {
        return m_data;
    }


    constexpr char const* CstrView::GetDataPtr() const
    {
        return &m_data;
    }
}


// ------------------------------------
// "span.hpp"


namespace Aiva
{
    template <typename TType>
    class Span final
    {
    public:
        constexpr Span();
        constexpr Span(size_t const size, TType& data);
        constexpr Span(size_t const size, TType *const data);
        constexpr Span(TType& data);
        constexpr Span(TType *const data);
        constexpr Span(nullptr_t const);

        constexpr operator bool() const;
        constexpr size_t GetSize() const;
        constexpr TType& operator[](size_t const index) const;
        constexpr TType& GetDataRef() const;
        constexpr TType* GetDataPtr() const;

    private:
        size_t m_size;
        TType* m_data;
    };
}


// ------------------------------------
// "span.inl"


namespace Aiva
{
    template <typename TType>
    constexpr Span<TType>::Span()
    {
        m_size = 0;
        m_data = nullptr;
    }


    template <typename TType>
    constexpr Span<TType>::Span(size_t const size, TType& data)
    {
        if (size > 0)
        {
            m_size = size;
            m_data = &data;
        }
        else
        {
            m_size = 0;
            m_data = nullptr;
        }
    }


    template <typename TType>
    constexpr Span<TType>::Span(size_t const size, TType *const data)
    {
        if (size > 0 && data)
        {
            m_size = size;
            m_data = data;
        }
        else
        {
            m_size = 0;
            m_data = nullptr;
        }
    }


    template <typename TType>
    constexpr Span<TType>::Span(TType& data)
    {
        m_size = 1;
        m_data = &data;
    }


    template <typename TType>
    constexpr Span<TType>::Span(TType *const data)
    {
        if (data)
        {
            m_size = 1;
            m_data = data;
        }
        else
        {
            m_size = 0;
            m_data = nullptr;
        }
    }


    template <typename TType>
    constexpr Span<TType>::Span(nullptr_t const)
    {
        m_size = 0;
        m_data = nullptr;
    }


    template <typename TType>
    constexpr Span<TType>::operator bool() const
    {
        return m_size > 0 && m_data;
    }


    template <typename TType>
    constexpr size_t Span<TType>::GetSize() const
    {
        return m_size;
    }


    template <typename TType>
    constexpr TType& Span<TType>::operator[](size_t const index) const
    {
        return m_data[index];
    }


    template <typename TType>
    constexpr TType& Span<TType>::GetDataRef() const
    {
        return *m_data;
    }


    template <typename TType>
    constexpr TType* Span<TType>::GetDataPtr() const
    {
        return m_data;
    }
}
