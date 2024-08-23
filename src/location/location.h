#pragma once

#include <cstddef>

class Location
{
  private:
    size_t m_Line;
    size_t m_Column;
    size_t m_RangeStart;
    size_t m_RangeEnd;

  public:
    Location() : m_Line(0), m_Column(0), m_RangeStart(0), m_RangeEnd(0) {};
    Location(size_t line, size_t column, size_t start, size_t end)
        : m_Line(line), m_Column(column), m_RangeStart(start), m_RangeEnd(end) {};

    ~Location() = default;

    size_t GetLine() const { return m_Line; }
    size_t GetColumn() const { return m_Column; }
    size_t GetStart() const { return m_RangeStart; }
    size_t GetEnd() const { return m_RangeEnd; }

    Location operator+(const Location &other) { return Location(m_Line, m_Column, m_RangeStart, other.GetEnd()); }
};
