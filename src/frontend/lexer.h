#pragma once

#include <cstddef>
#include <cstdio>
#include <functional>
#include <iostream>
#include <string>

#include "../diagnostics/diagnostics.h"
#include "token.h"

using Predicate = std::function<bool(char)>;

class Lexer
{
  private:
    size_t m_Cursor;
    size_t m_RangeStart;
    size_t m_RangeEnd;

    size_t m_Line;
    size_t m_Column;

    const std::string &m_Raw;
    const std::string &m_FileName;

    char peekOne();
    void advanceOne();
    void skipWhitespace();
    void updateRangeStart();
    bool isEof();

    Token readStringToken();
    Token readSimpleToken(TokenKind);

    std::string readWhile(Predicate);

    Diagnostics diagnostics;

  public:
    Lexer(const std::string &fileName, const std::string &raw) : m_FileName(fileName), m_Raw(raw), diagnostics()
    {
        m_Line = 1;
        m_Column = 1;
        m_Cursor = 0;
        m_RangeStart = 0;
    };

    Token GetNextToken();

    const std::string &GetFileContents() const { return m_Raw; };
    const std::string &GetFileName() const { return m_FileName; };
};
