#pragma once

#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

enum class TypeKind
{
    Void = 1,
    String,
    Integer,
    Object,
    Function,
};

class Type
{
  private:
    TypeKind m_Kind;

  public:
    Type(TypeKind kind) : m_Kind(kind) {};

    TypeKind GetKind() const { return m_Kind; }

    virtual std::string ToString() const
    {
        switch (m_Kind)
        {
        case TypeKind::String:
            return "string";
        case TypeKind::Integer:
            return "number";
        case TypeKind::Void:
            return "void";
        case TypeKind::Function:
            return "function";
        case TypeKind::Object:
            return "object";
        }
    };
};

class TypeObject : public Type
{
  private:
    std::optional<std::string> TypeName;
    std::unordered_map<std::string, std::shared_ptr<Type>> m_Entries;

  public:
    TypeObject(std::unordered_map<std::string, std::shared_ptr<Type>> entries, std::optional<std::string> typeName)
        : Type(TypeKind::Object), m_Entries(std::move(entries)), TypeName(typeName) {};

    std::shared_ptr<Type> GetField(std::string key) const;

    // TODO: support user defined object names
    std::string ToString() const override { return TypeName.value(); };
};

class TypeFunction : public Type
{
  public:
    const bool IsVarArgs;
    const std::shared_ptr<Type> ReturnType;
    const std::vector<std::unique_ptr<Type>> Parameters;

    TypeFunction(bool isVarArgs, std::shared_ptr<Type> returnType, std::vector<std::unique_ptr<Type>> params)
        : Type(TypeKind::Function), IsVarArgs(isVarArgs), ReturnType(returnType), Parameters(std::move(params)) {};

    std::string ToString() const override { return "() -> void"; };
};
