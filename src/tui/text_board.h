#pragma once

#include <stddef.h>

#include "collections/gap_buffer.h"
#include "tui/board.h"
#include "tui/context.h"

void print_gap_buffer_to_board(
    struct Board           *text_board,
    const struct GapBuffer *gb,
    size_t                  starting_index,
    size_t                  max_rows,
    size_t                  max_columns
);

void update_text_board(struct TuiContext *tctx, struct Board *text_board);
