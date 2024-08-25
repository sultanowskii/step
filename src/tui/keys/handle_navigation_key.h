#pragma once

#include <ncurses.h>
#include <stddef.h>

#include "collections/gap_buffer.h"
#include "tui/boards/board.h"
#include "tui/context.h"
#include "tui/coords.h"

void handle_navigation_key(
    struct TuiContext *tctx,
    struct Board      *text_board,
    int                c
);
