#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>
#include <variant>

#include "lexer.h"
#include "token.h"

Token Lexer::GetNextToken()
{
    skipWhitespace();
    updateRangeStart();

    if (isEof())
    {
        return Token(TokenKind::Eof, std::monostate(), Location(m_Line, m_Column, m_RangeStart, m_RangeEnd));
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
    case ',':
        return readSimpleToken(TokenKind::Comma);
    case '=':
        return readSimpleToken(TokenKind::Equal);
    case '"':
        return readStringToken();
    }

    if (std::isalpha(currentChar) || '_' == currentChar)
    {
        size_t startLine = m_Line;
        size_t startColumn = m_Column;
        std::string label = readWhile([](char x) { return std::isalnum(x) || '_' == x; });

        if ("let" == label)
        {
            return Token(TokenKind::Let, std::monostate(), Location(startLine, startColumn, m_RangeStart, m_RangeEnd));
        }

        return Token(TokenKind::Identifier, label, Location(startLine, startColumn, m_RangeStart, m_RangeEnd));
    }

    if (std::isdigit(currentChar))
    {
        size_t startLine = m_Line;
        size_t startColumn = m_Column;
        long double number = std::stold(readWhile([](char x) { return std::isdigit(x) || x == '.'; }));
        return Token(TokenKind::Number, number, Location(startLine, startColumn, m_RangeStart, m_RangeEnd));
    }

    abort();
};

Token Lexer::readSimpleToken(TokenKind kind)
{
    size_t startLine = m_Line;
    size_t startColumn = m_Column;
    advanceOne();
    return Token(kind, std::monostate(), Location(startLine, startColumn, m_RangeStart, m_RangeEnd));
}

Token Lexer::readStringToken()
{
    size_t startLine = m_Line;
    size_t startColumn = m_Column;

    advanceOne(); // eat left '"'

    std::string label;

    size_t start = m_Cursor;

    while (1)
    {
        if (peekOne() == '"')
        {
            label = m_Raw.substr(start, m_Cursor - start);
            advanceOne(); // eat right '"'
            break;
        }

        if (isEof() || peekOne() == '\n')
        {
            DiagnosticError err(ErrorKind::UnterminatedString,
                                Location(startLine, startColumn, m_RangeStart, m_RangeEnd),
                                "Unterminated string literal");
            diagnostics.EmitNow(std::cerr, m_FileName, m_Raw, err);
            abort();
        }

        advanceOne();
    }

    return Token(TokenKind::String, label, Location(startLine, startColumn, m_RangeStart, m_RangeEnd));
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
char Lexer::peekOne() { return isEof() ? '\00' : m_Raw.at(m_Cursor); }
void Lexer::updateRangeStart() { m_RangeStart = m_Cursor; }
