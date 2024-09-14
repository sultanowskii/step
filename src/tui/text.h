#pragma once

#include <ncurses.h>
#include <stdbool.h>
#include <stddef.h>

#include "collections/gap_buffer.h"
#include "tui/boards/board.h"
#include "tui/coords.h"
#include "tui/core/context.h"
#include "tui/optionals.h"

optional_coords next_valid_coords(
    const struct Coords *coords,
    size_t               max_rows,
    size_t               max_columns,
    char                 symbol
);

void          revise_cursor(struct TuiContext *tctx, size_t max_rows, size_t max_columns);
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

optional_size_t get_index_from_cursor_position(const struct TuiContext *tctx, size_t max_rows, size_t max_columns);
optional_size_t get_index_from_position(
    const struct GapBuffer *gb,
    size_t                  starting_index,
    size_t                  max_rows,
    size_t                  max_columns,
    const struct Coords    *position
);

bool            move_cursor_to_index(const struct TuiContext *tctx, size_t max_rows, size_t max_columns, size_t target_index);
optional_coords get_position_from_index(
    const struct GapBuffer *gb,
    size_t                  starting_index,
    size_t                  max_rows,
    size_t                  max_columns,
    size_t                  target_index
);
