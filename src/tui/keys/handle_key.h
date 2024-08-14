#pragma once

#include "tui/boards/board.h"
#include "tui/context.h"

void handle_key(
    struct TuiContext *tctx,
    struct Board      *line_number_board,
    struct Board      *status_board,
    struct Board      *text_board,
    int                key
);
