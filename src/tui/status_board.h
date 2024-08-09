#pragma once

#include <ncurses.h>
#include <stddef.h>

#include "tui/board.h"
#include "tui/coords.h"

void update_status_board(
    struct Board        *status_board,
    size_t               line_index,
    const struct Coords *cursor
);
