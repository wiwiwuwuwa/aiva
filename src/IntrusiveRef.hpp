#pragma once
#include "RefCountObjectHolder.hpp"

namespace Aiva
{
    template <typename TObject>
    class IntrusiveRef final
    {
    public:
        template <typename... TArgs>
        IntrusiveRef(TArgs&&... args);

        IntrusiveRef(IntrusiveRef const& other);
        IntrusiveRef& operator=(IntrusiveRef const& other);
        IntrusiveRef(IntrusiveRef&& other);
        IntrusiveRef& operator=(IntrusiveRef&& other);
        ~IntrusiveRef();

        TObject* operator->() const;

    private:
        using ObjectHolder_t = RefCountObjectHolder<TObject>;
        ObjectHolder_t* m_objectHolder{};
    };
}
// namespace Aiva


#include "IntrusiveRef.inl"
