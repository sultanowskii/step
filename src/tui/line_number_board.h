#pragma once

#include <stddef.h>

#include "collections/gap_buffer.h"
#include "tui/board.h"

void print_line_numbers_to_board(
    const struct GapBuffer *gb,
    struct Board           *line_number_board,
    size_t                  starting_index,
    size_t                  max_rows,
    size_t                  max_columns,
    size_t                  starting_line_index
);
