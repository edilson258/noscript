#pragma once

#include <cstddef>
#include <string>

enum Color
{
    Cyan = 1,
    RedBold,
    BrownLow,
};

class Painter
{
  public:
    static std::string Paint(std::string text, Color color);
    static std::string HighlightRange(const std::string &haystack, size_t begin, size_t end, std::string customColor);
};
