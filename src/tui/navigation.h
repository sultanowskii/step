#pragma once

#include <ncurses.h>
#include <stddef.h>

#include "tui/coords.h"

void handle_navigation_key(
    int            c,
    struct Coords *cursor,
    WINDOW        *text_window,
    size_t         text_window_height,
    size_t         text_window_width
);
