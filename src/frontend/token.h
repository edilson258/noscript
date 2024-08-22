#pragma once

#include <cstddef>
#include <ostream>
#include <string>
#include <tuple>
#include <variant>

#include "../range/range.h"

enum class TokenKind
{
    Eof = 1,

    String,

    Identifier,

    Dot,
    Semicolon,
    LeftParent,
    RightParent,
};

using TokenData = std::variant<std::monostate, std::string>;

class Token
{
  private:
    TokenKind m_Kind;
    TokenData m_Data;
    Range m_Range;

  public:
    Token() = default;
    Token(TokenKind kind, TokenData data, Range range) : m_Kind(kind), m_Data(data), m_Range(range) {};

    ~Token() = default;

    Range GetRange() const { return m_Range; }
    TokenKind GetKind() const { return m_Kind; }
    TokenData GetData() const { return m_Data; }
};

std::ostream &operator<<(std::ostream &, const Token &);
std::ostream &operator<<(std::ostream &os, const TokenKind &);
