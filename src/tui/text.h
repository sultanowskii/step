#pragma once

#include <ncurses.h>
#include <stddef.h>

#include "collections/gap_buffer.h"

void gap_buffer_print_to_window(
    const struct GapBuffer *gb,
    WINDOW                 *win,
    size_t                  starting_index,
    size_t                  max_rows,
    size_t                  max_columns
);
