#pragma once

#include <functional>
#include <iostream>
#include <string>

#include "token.h"

using Predicate = std::function<bool(char)>;

class Lexer
{
  private:
    bool m_IsEof;
    size_t m_Line;
    size_t m_Column;
    size_t m_Cursor;
    size_t m_ReadCursor;
    char m_CurrentChar;
    const std::string &m_FileName;
    const std::string &m_Raw;

    std::string readWhile(Predicate);
    void skipWhitespace();
    void readChar();

  public:
    Lexer(const std::string &fileName, const std::string &raw) : m_FileName(fileName), m_Raw(raw)
    {
        m_IsEof = raw.length() == 0;
        m_Line = 1;
        m_Column = 0;
        m_Cursor = 0;
        m_ReadCursor = 0;
        m_CurrentChar = 0;

        readChar(); // fill 'm_CurrentChar'
    };

    Token GetNextToken();
};
