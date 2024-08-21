#include "scope.h"

std::shared_ptr<Type> Scope::Resolve(std::string key) const
{
    if (m_Store.find(key) != m_Store.end())
    {
        return m_Store.at(key);
    }

    if (HasParent())
    {
        return m_Parent.get()->Resolve(key);
    }

    return nullptr;
}
