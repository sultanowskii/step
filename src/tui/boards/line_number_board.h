#pragma once

#include <stddef.h>

#include "collections/gap_buffer.h"
#include "tui/boards/board.h"
#include "tui/context.h"

void update_line_number_board(
    struct TuiContext *tctx,
    struct Board      *line_number_board,
    size_t             max_rows,
    size_t             max_columns
);
