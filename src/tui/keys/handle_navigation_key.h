#pragma once

#include <ncurses.h>
#include <stddef.h>

#include "collections/gap_buffer.h"
#include "tui/boards/board.h"
#include "tui/coords.h"
#include "tui/core/context.h"

void handle_navigation_key(
    struct TuiContext *tctx,
    int                c
);
