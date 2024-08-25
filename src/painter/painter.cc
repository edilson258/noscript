#include <sstream>
#include <string>

#include "painter.h"

#define COLOR_RESET "\x1b[0m"
#define COLOR_RED_BOLD "\x1b[1;31m"
#define COLOR_RED_UNDERLINED "\x1b[4;31m"
#define COLOR_CYAN "\x1b[36m"
#define COLOR_BROW_LOW "\x1b[38;5;8m"

std::string Painter::Paint(std::string text, Color colorKind)
{
    switch (colorKind)
    {
    case Color::RedBold:
        text.insert(0, COLOR_RED_BOLD);
        break;
    case Color::Cyan:
        text.insert(0, COLOR_CYAN);
        break;
    case Color::BrownLow:
        text.insert(0, COLOR_BROW_LOW);
        break;
    }
    text.append(COLOR_RESET);

    return text;
}

std::string Painter::HighlightRange(const std::string &code, size_t begin, size_t end, std::string customFill)
{
    size_t code_len = code.length();
    end = end >= code_len ? code_len - 1 : end;

    size_t cursor = 0;
    size_t line_counter = 1;
    size_t begin_of_line = 0;

    size_t index_where_slice_begin = 0;
    size_t line_number_where_slice_begin = 0;
    size_t line_number_where_slice_end = 0;

    while (cursor < code_len)
    {
        char curr_char = code[cursor];

        if (cursor == begin)
        {
            line_number_where_slice_begin = line_counter;
            index_where_slice_begin = begin_of_line;
        }

        if (curr_char == '\n')
        {
            line_counter += 1;
            begin_of_line = cursor + 1; // +1 to skipe '\n'

            if (cursor >= end)
            {
                line_number_where_slice_end = line_counter;
                break;
            }
        }

        cursor++;
    }

    std::ostringstream oss;

    oss << "    |" << std::endl;

    cursor = index_where_slice_begin;
    for (size_t i = line_number_where_slice_begin; i < line_number_where_slice_end; ++i)
    {
        oss << "  " << i << " | ";
        while (code[cursor] != '\n')
        {
            oss << code[cursor];
            cursor++;
        }

        oss << code[cursor++];
        oss << "    | ";

        for (size_t j = index_where_slice_begin; j <= end; ++j)
        {
            if (j >= begin && j <= end)
            {
                // under the text to highlight
                oss << customFill;
            }
            else
            {
                oss << " ";
            }
        }
        oss << std::endl;
    }

    return oss.str();
}
