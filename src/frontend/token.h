#pragma once

#include <cstddef>
#include <ostream>
#include <string>
#include <tuple>
#include <variant>

#include "../location/location.h"

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
    Location m_Location;

  public:
    Token() = default;
    Token(TokenKind kind, TokenData data, Location loc) : m_Kind(kind), m_Data(data), m_Location(loc) {};

    ~Token() = default;

    TokenKind GetKind() const { return m_Kind; }
    TokenData GetData() const { return m_Data; }
    Location GetLocation() const { return m_Location; }
};

std::ostream &operator<<(std::ostream &, const Token &);
std::ostream &operator<<(std::ostream &os, const TokenKind &);
