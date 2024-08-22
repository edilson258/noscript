#include "token.h"

std::ostream &operator<<(std::ostream &os, const Token &token)
{

    switch (token.kind)
    {
    case TokenKind::Eof:
        os << "Eof";
        break;
    case TokenKind::Dot:
        os << "Symbol: .";
        break;
    case TokenKind::LeftParent:
        os << "Symbol: (";
        break;
    case TokenKind::RightParent:
        os << "Symbol: )";
        break;
    case TokenKind::Semicolon:
        os << "Symbol: ;";
        break;
    case TokenKind::String:
        os << "String: " << std::get<std::string>(token.data);
        break;
    case TokenKind::Identifier:
        os << "Identifier: " << std::get<std::string>(token.data);
        break;
    }

    os << " range " << token.range.Start << ":" << token.range.End;

    return os;
}

std::ostream &operator<<(std::ostream &os, const TokenKind &tokenKind)
{

    switch (tokenKind)
    {
    case TokenKind::Eof:
        os << "Eof";
        break;
    case TokenKind::Dot:
        os << "Symbol: .";
        break;
    case TokenKind::LeftParent:
        os << "Symbol: (";
        break;
    case TokenKind::RightParent:
        os << "Symbol: )";
        break;
    case TokenKind::Semicolon:
        os << "Symbol: ;";
        break;
    case TokenKind::String:
        os << "String Literal";
        break;
    case TokenKind::Identifier:
        os << "Identifier";
        break;
    }

    return os;
}
