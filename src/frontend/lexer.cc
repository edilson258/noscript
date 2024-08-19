#include <cctype>
#include <cstddef>
#include <cstdlib>

#include "lexer.h"
#include "token.h"

Token Lexer::GetNextToken()
{
    skipWhitespace();

    Token token;
    token.pos.line = m_Line;
    token.pos.column = m_Column;

    if (m_IsEof)
    {
        token.kind = TokenKind::Eof;
        return token;
    }

    switch (m_CurrentChar)
    {
    case '(':
        readChar();
        token.kind = TokenKind::LeftParent;
        return token;
    case ')':
        readChar();
        token.kind = TokenKind::RightParent;
        return token;
    case '.':
        readChar();
        token.kind = TokenKind::Dot;
        return token;
    case ';':
        readChar();
        token.kind = TokenKind::Semicolon;
        return token;
    case '"':
        readChar();
        token.kind = TokenKind::String;
        token.data.emplace<std::string>(readWhile([](char x) { return x != '"'; }));
        readChar();
        return token;
    }

    if (std::isalpha(m_CurrentChar) || '_' == m_CurrentChar)
    {
        std::string label = readWhile([](char x) { return std::isalnum(x) || x == '_'; });
        token.kind = TokenKind::Identifier;
        token.data.emplace<std::string>(label);
        return token;
    }

    std::cerr << "Error: unecpected token: " << m_CurrentChar;
    std::cerr << " at " << m_Line << ":" << m_Column << std::endl;
    abort();
}

std::string Lexer::readWhile(Predicate predicate)
{
    size_t start = m_Cursor;
    while (!m_IsEof && predicate(m_CurrentChar))
    {
        readChar();
    }
    return m_Raw.substr(start, m_Cursor - start);
}

void Lexer::skipWhitespace()
{
    while (!m_IsEof && std::isspace(m_CurrentChar))
    {
        readChar();
    }
}

void Lexer::readChar()
{
    m_Cursor = m_ReadCursor;

    if (m_Cursor >= m_Raw.length())
    {
        m_IsEof = true;
        return;
    }

    m_CurrentChar = m_Raw.at(m_Cursor);
    m_ReadCursor++;

    if (m_CurrentChar == '\n')
    {
        m_Line++;
        m_Column = 0;
    }
    else
    {
        m_Column++;
    }
}
