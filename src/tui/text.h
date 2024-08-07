#pragma once

#include <ncurses.h>
#include <stddef.h>

#include "board.h"
#include "collections/gap_buffer.h"
#include "tui/coords.h"

void gap_buffer_print_to_board(
    const struct GapBuffer *gb,
    struct Board           *board,
    size_t                  starting_index,
    size_t                  max_rows,
    size_t                  max_columns
);

struct Coords gap_buffer_revise_coords(
    const struct GapBuffer *gb,
    size_t                  starting_index,
    size_t                  max_rows,
    size_t                  max_columns,
    struct Coords           raw
);

void print_line_numbers_to_board(
    const struct GapBuffer *gb,
    struct Board           *line_number_board,
    size_t                  starting_index,
    size_t                  max_rows,
    size_t                  max_columns,
    size_t                  starting_line_index
);
