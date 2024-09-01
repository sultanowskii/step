#pragma once

#include <stddef.h>

#include "collections/gap_buffer.h"
#include "tui/boards/board.h"
#include "tui/core/context.h"

void update_line_number_board(
    struct TuiContext *tctx,
    size_t             max_rows,
    size_t             max_columns
);
