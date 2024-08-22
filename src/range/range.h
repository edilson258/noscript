#pragma once

#include <cstddef>

class Range
{
  public:
    size_t Start;
    size_t End;

    Range() : Start(0), End(0) {};
    Range(size_t start, size_t end) : Start(start), End(end) {};
    ~Range() = default;

    Range operator+(const Range &rhs)
    {
        Range range;
        range.Start = Start;
        range.End = rhs.End;
        return range;
    }
};
