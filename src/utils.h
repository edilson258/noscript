#pragma once

#include <string>

std::string highlightError(std::string &, int start, int end);
std::string highlight(std::string &, int start, int end, std::string color);
