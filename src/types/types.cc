#include <memory>
#include <string>

#include "types.h"

std::shared_ptr<Type> TypeObject::GetField(std::string key) const
{
    if (m_Entries.find(key) != m_Entries.end())
    {
        return m_Entries.at(key);
    }

    return nullptr;
}
