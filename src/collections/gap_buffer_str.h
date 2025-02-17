#pragma once

#include <stddef.h>

#include "collections/gap_buffer.h"

size_t gap_buffer_count_symbol(const struct GapBuffer *gb, char symbol);
size_t gap_buffer_count_lines(const struct GapBuffer *gb);
size_t gap_buffer_calculate_line_count_digit_count(const struct GapBuffer *gb);
