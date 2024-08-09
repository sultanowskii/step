#pragma once

#include <stddef.h>

#include "collections/gap_buffer.h"
#include "tui/board.h"

void update_line_number_board(
    struct Board           *line_number_board,
    const struct GapBuffer *gb,
    size_t                  starting_index,
    size_t                  max_rows,
    size_t                  max_columns,
    size_t                  starting_line_index
);
