#pragma once

#include <ncurses.h>
#include <panel.h>
#include <stddef.h>

#include "tui/coords.h"

// Wrapper around PANEL.
struct Board {
    PANEL        *panel;
    size_t        height;
    size_t        width;
    struct Coords start;
};

// "Creates" board, initialises window and panel
struct Board create_board(size_t height, size_t width, size_t offset_y, size_t offset_x);
// "Destroys" board, deletes window and panel
void board_destroy(struct Board *board);

PANEL  *board_panel(const struct Board *board);
WINDOW *board_window(const struct Board *board);
