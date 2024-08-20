#pragma once

#include <cstddef>
#include <ostream>
#include <string>
#include <variant>

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

class TokenPosition
{
  public:
    size_t line;
    size_t column;

    TokenPosition() : line(0), column(0) {};
    TokenPosition(size_t line, size_t column) : line(line), column(column) {};

    ~TokenPosition() = default;
};

class Token
{
  public:
    TokenKind kind;
    TokenData data;
    TokenPosition pos;

    Token() = default;
    ~Token() = default;
};

std::ostream &operator<<(std::ostream &, const Token &);
std::ostream &operator<<(std::ostream &os, const TokenKind &);
