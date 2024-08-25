#include "token.h"

std::ostream &operator<<(std::ostream &os, const Token &token)
{
    switch (token.GetKind())
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
        os << "String: " << std::get<std::string>(token.GetData());
        break;
    case TokenKind::Number:
        os << "Number: " << std::get<long double>(token.GetData());
        break;
    case TokenKind::Identifier:
        os << "Identifier: " << std::get<std::string>(token.GetData());
        break;
    }

    Location loc = token.GetLocation();
    os << " line: " << loc.GetLine() << " column: " << loc.GetColumn();
    os << " start: " << loc.GetStart() << " end: " << loc.GetEnd();

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
    case TokenKind::Number:
        os << "Number Literal";
        break;
    case TokenKind::Identifier:
        os << "Identifier";
        break;
    }

    return os;
}
