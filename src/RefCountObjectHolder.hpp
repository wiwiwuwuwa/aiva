#pragma once
#include "RefCountObject.hpp"


namespace Aiva
{
    template <typename TObject>
    class RefCountObjectHolder final : public RefCountObject
    {
    public:
        template <typename... TArgs>
        RefCountObjectHolder(TArgs&&... args);

        TObject& GetObject();
        TObject const& GetObject() const;

    private:
        TObject m_object;
    };
}
// namespace Aiva


#include "RefCountObjectHolder.inl"
