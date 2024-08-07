#pragma once

#include <ncurses.h>
#include <stddef.h>

#include "tui/board.h"
#include "tui/coords.h"

void handle_navigation_key(
    int            c,
    struct Coords *cursor,
    struct Board  *text_board
);
