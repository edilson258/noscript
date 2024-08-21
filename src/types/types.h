#pragma once

#include <memory>
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
};

class TypeObject : public Type
{
  private:
    std::unordered_map<std::string, std::shared_ptr<Type>> m_Entries;

  public:
    TypeObject(std::unordered_map<std::string, std::shared_ptr<Type>> entries)
        : Type(TypeKind::Object), m_Entries(std::move(entries)) {};

    std::shared_ptr<Type> GetField(std::string key) const;
};

class TypeFunction : public Type
{
  public:
    const bool IsVarArgs;
    const std::shared_ptr<Type> ReturnType;
    const std::vector<std::unique_ptr<Type>> Parameters;

    TypeFunction(bool isVarArgs, std::shared_ptr<Type> returnType, std::vector<std::unique_ptr<Type>> params)
        : Type(TypeKind::Function), IsVarArgs(isVarArgs), ReturnType(returnType), Parameters(std::move(params)) {};
};
