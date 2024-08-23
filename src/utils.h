#pragma once

#include <string>

#define COLOR_RESET "\x1b[0m"
#define COLOR_RED_UNDERLINED "\x1b[4m\x1b[31m"

std::string highlightError(const std::string &, int start, int end);
std::string highlight(const std::string &, int start, int end, std::string color);
