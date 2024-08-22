#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <variant>

#include "../utils.h"
#include "lexer.h"
#include "token.h"

Token Lexer::GetNextToken()
{
    skipWhitespace();
    updateRangeStart();

    if (isEof())
    {
        return Token(TokenKind::Eof, std::monostate(), createRange());
    }

    char currentChar = peekOne();

    switch (currentChar)
    {
    case '(':
        return readSimpleToken(TokenKind::LeftParent);
    case ')':
        return readSimpleToken(TokenKind::RightParent);
    case '.':
        return readSimpleToken(TokenKind::Dot);
    case ';':
        return readSimpleToken(TokenKind::Semicolon);
    case '"':
        return readStringToken();
    }

    if (std::isalpha(currentChar) || '_' == currentChar)
    {
        auto label = readWhile([](char x) { return std::isalnum(x) || '_' == x; });
        Token identifier = Token(TokenKind::Identifier, label, createRange());
        return identifier;
    }

    abort();
};

Token Lexer::readSimpleToken(TokenKind kind)
{
    advanceOne();
    Token simpleToken = Token(kind, std::monostate(), createRange());
    return simpleToken;
}

Token Lexer::readStringToken()
{
    advanceOne(); // eat '"'

    std::string label;

    size_t start = m_Cursor;

    while (1)
    {

        if (peekOne() == '"')
        {
            label = m_Raw.substr(start, m_Cursor - start);
            advanceOne(); // eat '"'
            break;
        }

        if (isEof() || peekOne() == '\n')
        {
            std::cerr << "\033[1m\x1b[31mERROR\x1b[0m: unterminated string literal" << std::endl;
            std::cerr << highlightError(m_Raw, start - 1, m_Cursor); // 'start - 1' to highlight the '"'
            abort();
        }

        advanceOne();
    }

    return Token(TokenKind::String, label, createRange());
}

std::string Lexer::readWhile(Predicate p)
{
    size_t start = m_Cursor;
    while (!isEof() && p(peekOne()))
    {
        advanceOne();
    }
    return m_Raw.substr(start, m_Cursor - start);
}

void Lexer::skipWhitespace()
{
    while (!isEof() && std::isspace(peekOne()))
    {
        advanceOne();
    }
}

void Lexer::advanceOne()
{
    if (isEof())
    {
        return;
    }

    m_RangeEnd = m_Cursor;
    m_Cursor += 1;

    if (peekOne() == '\n')
    {
        m_Line += 1;
        m_Column = 0;
    }
    else
    {
        m_Column += 1;
    }
}

bool Lexer::isEof() { return m_Cursor >= m_Raw.length(); };
char Lexer::peekOne() { return m_Cursor < m_Raw.length() ? m_Raw.at(m_Cursor) : -1; }
Range Lexer::createRange() { return Range(m_RangeStart, m_RangeEnd); }
void Lexer::updateRangeStart() { m_RangeStart = m_Cursor; }
