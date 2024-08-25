#pragma once

#include <algorithm>
#include <memory>
#include <string>
#include <unordered_map>

#include "../types/types.h"

enum class ScopeKind
{
    Global = 1
};

class Scope
{
  private:
    ScopeKind m_Kind;
    std::unique_ptr<Scope> m_Parent;
    std::unordered_map<std::string, std::shared_ptr<Type>> m_Store;

  public:
    Scope(std::unordered_map<std::string, std::shared_ptr<Type>> entries)
        : m_Kind(ScopeKind::Global), m_Parent(nullptr), m_Store(entries) {};

    ScopeKind GetKind() const { return m_Kind; };
    bool HasParent() const { return m_Parent != nullptr; };
    void Assign(std::string key, std::shared_ptr<Type> value) { m_Store[key] = value; };
    std::shared_ptr<Type> Resolve(std::string) const;
};
