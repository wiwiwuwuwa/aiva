#include "RefCountObject.hpp"


using namespace Aiva;


void RefCountObject::IncRef()
{
    m_refs++;
}


void RefCountObject::DecRef()
{
    m_refs--;
}


bool RefCountObject::HasRef()
{
    return m_refs >= 1;
}
