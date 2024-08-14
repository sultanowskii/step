#pragma once

#include "tui/boards/board.h"
#include "tui/context.h"

void handle_other(
    struct TuiContext *tctx,
    struct Board      *text_board,
    int                key
);
