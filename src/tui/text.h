#pragma once

#include <ncurses.h>
#include <stddef.h>

#include "board.h"
#include "collections/gap_buffer.h"
#include "tui/coords.h"

void print_gap_buffer_to_board(
    const struct GapBuffer *gb,
    struct Board           *board,
    size_t                  starting_index,
    size_t                  max_rows,
    size_t                  max_columns
);

struct Coords revise_coords_with_gap_buffer(
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

struct FindLineResult {
    size_t index;
    bool   found;
};

struct FindLineResult find_next_line(const struct GapBuffer *gb, size_t starting_index);
struct FindLineResult find_previous_line(const struct GapBuffer *gb, size_t starting_index);
