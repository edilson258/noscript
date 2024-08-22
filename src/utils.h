#pragma once

#include <string>

std::string highlightError(const std::string &, int start, int end);
std::string highlight(const std::string &, int start, int end, std::string color);
