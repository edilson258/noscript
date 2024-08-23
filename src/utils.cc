#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>

#include "utils.h"

std::string highlightError(const std::string &code, int start, int end)
{
    return highlight(code, start, end, COLOR_RED_UNDERLINED);
}

std::string highlight(const std::string &code, int start, int end, std::string color)
{
    int code_len = code.length();
    end = end >= code_len ? code_len - 1 : end;

    int cursor = 0;
    int line_counter = 1;
    int begin_of_line = 0;

    int index_where_slice_begin = 0;
    int line_number_where_slice_begin = 0;
    int line_number_where_slice_end = 0;

    while (cursor < code_len)
    {
        char curr_char = code[cursor];

        if (cursor == start)
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
    for (int i = line_number_where_slice_begin; i < line_number_where_slice_end; ++i)
    {
        oss << "  " << i << " | ";
        while (code[cursor] != '\n')
        {
            if (cursor >= start && cursor <= end)
            {
                oss << color << code[cursor] << COLOR_RESET;
            }
            else
            {
                oss << code[cursor];
            }
            cursor++;
        }
        // append '\n' and incr cursor
        oss << code[cursor++];
    }

    oss << "    |" << std::endl;

    return oss.str();
}
