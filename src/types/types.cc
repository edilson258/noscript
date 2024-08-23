#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include "types.h"

std::ostream &operator<<(std::ostream &os, const Type &type) { return type.Dump(os); }

std::ostream &Type::Dump(std::ostream &os) const
{
    // Note that non trivial types like: `TypeObject`, `TypeFunction` will override and display as expected

    switch (m_Kind)
    {
    case TypeKind::Void:
        os << "void";
        break;
    case TypeKind::String:
        os << "string";
        break;
    case TypeKind::Integer:
        os << "integer";
        break;
    case TypeKind::Object:
        os << "object";
        break;
    case TypeKind::Function:
        os << "function";
        break;
    }

    return os;
}

std::ostream &TypeObject::Dump(std::ostream &os) const
{
    if (m_TypeName.has_value())
    {
        os << m_TypeName.value();
        return os;
    }

    os << "{";

    int i = 0;
    for (std::pair<std::string, std::shared_ptr<Type>> entry : Entries)
    {
        os << entry.first;
        os << ":";
        os << *entry.second.get();

        if (i + 1 < Entries.size())
        {
            os << ",";
        }

        i++;
    }

    os << "}";

    return os;
}

std::ostream &TypeFunction::Dump(std::ostream &os) const
{
    os << "(";

    int i = 0;
    for (std::shared_ptr<Type> paramType : m_Parameters)
    {
        os << *paramType.get();
        if (i + 1 < m_Parameters.size())
        {
            os << ", ";
        }
        i++;
    }

    if (m_IsVarArgs)
    {
        if (m_Parameters.size() > 0)
        {
            os << ", any[] ...";
        }
        else
        {
            os << "any[] ...";
        }
    }

    os << ")";

    os << " -> ";
    os << *m_ReturnType.get();

    return os;
}

bool Type::operator==(const Type &other) const
{
    // Note that non trivial types will override to make a deep comparison
    return m_Kind == other.GetKind();
}

bool TypeObject::operator==(const Type &other) const
{
    // both objects must be deeply equal, not allowing subsets or supersets

    if (TypeKind::Object != other.GetKind())
    {
        return false;
    }

    const TypeObject otherObject = static_cast<const TypeObject &>(other);

    if (Entries.size() != otherObject.Entries.size())
    {
        return false;
    }

    for (const std::pair<std::string, std::shared_ptr<Type>> entry : Entries)
    {
        for (const std::pair<std::string, std::shared_ptr<Type>> otherEntry : otherObject.Entries)
        {
            if (entry.first != otherEntry.first || !(*entry.second.get() == *otherEntry.second.get()))
            {
                return false;
            }
        }
    }

    return true;
}

bool TypeFunction::operator==(const Type &other) const
{
    if (TypeKind::Function != other.GetKind())
    {
        return false;
    }

    const TypeFunction &otherFunction = static_cast<const TypeFunction &>(other);

    if (m_IsVarArgs != otherFunction.m_IsVarArgs)
    {
        return false;
    }

    if (!(*m_ReturnType.get() == *otherFunction.m_ReturnType.get()))
    {
        return false;
    }

    if (m_Parameters.size() != otherFunction.m_Parameters.size())
    {
        return false;
    }

    for (const std::shared_ptr<Type> paramType : m_Parameters)
    {
        for (const std::shared_ptr<Type> otherParamType : otherFunction.m_Parameters)
        {
            if (!(*paramType.get() == *otherParamType.get()))
            {
                return false;
            }
        }
    }

    return true;
}

std::shared_ptr<Type> TypeObject::GetField(std::string key) const
{
    if (Entries.find(key) != Entries.end())
    {
        return Entries.at(key);
    }

    return nullptr;
}
