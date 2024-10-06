#pragma once

#include <ncurses.h>
#include <stdbool.h>
#include <stddef.h>

#include "collections/gap_buffer.h"
#include "nonstd/optionals.h"
#include "tui/boards/board.h"
#include "tui/coords.h"
#include "tui/optionals.h"

size_t gap_buffer_get_max_valid_index(const struct GapBuffer *gb);

struct Coords revise_coords_with_gap_buffer(
    const struct GapBuffer *gb,
    size_t                  starting_index,
    size_t                  max_rows,
    size_t                  max_columns,
    struct Coords           raw
);

struct FindLineResult {
    size_t index;
    bool   found;
};

struct FindLineResult find_start_of_next_line(const struct GapBuffer *gb, size_t starting_index);
struct FindLineResult find_start_of_previous_line(const struct GapBuffer *gb, size_t starting_index);

optional_size_t get_index_from_position(
    const struct GapBuffer *gb,
    size_t                  starting_index,
    size_t                  max_rows,
    size_t                  max_columns,
    const struct Coords    *position
);

optional_coords get_position_from_index(
    const struct GapBuffer *gb,
    size_t                  starting_index,
    size_t                  max_rows,
    size_t                  max_columns,
    size_t                  target_index
);

size_t first_y_of_line_under_pos(
    struct GapBuffer    *gb,
    size_t               starting_symbol_index,
    size_t               text_board_max_rows,
    size_t               text_board_max_columns,
    const struct Coords *pos
);
size_t last_y_of_line_under_pos(
    struct GapBuffer    *gb,
    size_t               starting_symbol_index,
    size_t               text_board_max_rows,
    size_t               text_board_max_columns,
    const struct Coords *pos
);
