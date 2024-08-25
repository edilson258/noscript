#pragma once

#include <memory>
#include <optional>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

enum class TypeKind
{
    Void = 1,
    String,
    Number,
    Object,
    Function,
};

class Type
{
  private:
    TypeKind m_Kind;

  public:
    Type(TypeKind kind) : m_Kind(kind) {};

    virtual ~Type() = default;

    TypeKind GetKind() const { return m_Kind; }

    virtual bool operator==(const Type &other) const;
    virtual std::ostream &Dump(std::ostream &) const;

    friend std::ostream &operator<<(std::ostream &, const Type &);
};

class TypeObject : public Type
{
  private:
    std::optional<std::string> m_TypeName;

  public:
    // TODO: make this private
    const std::unordered_map<std::string, std::shared_ptr<Type>> Entries;

    TypeObject(std::unordered_map<std::string, std::shared_ptr<Type>> entries, std::optional<std::string> typeName)
        : Type(TypeKind::Object), m_TypeName(typeName), Entries(entries) {};

    ~TypeObject() = default;

    std::shared_ptr<Type> GetField(std::string key) const;

    bool operator==(const Type &other) const override;
    std::ostream &Dump(std::ostream &) const override;
};

class TypeFunction : public Type
{
  private:
    bool m_IsVarArgs;
    std::shared_ptr<Type> m_ReturnType;
    std::vector<std::shared_ptr<Type>> m_Parameters;

  public:
    TypeFunction(bool isVarArgs, std::shared_ptr<Type> returnType, std::vector<std::shared_ptr<Type>> params)
        : Type(TypeKind::Function), m_IsVarArgs(isVarArgs), m_ReturnType(returnType), m_Parameters(params) {};

    ~TypeFunction() = default;

    bool operator==(const Type &other) const override;
    std::ostream &Dump(std::ostream &) const override;

    bool GetIsVarArgs() const { return m_IsVarArgs; }
    const std::shared_ptr<Type> GetReturnType() const { return m_ReturnType; }
    const std::vector<std::shared_ptr<Type>> GetParamaters() const { return m_Parameters; }
};
