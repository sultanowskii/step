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

struct Board *board_create_empty();
// Creates uninitialized board. Must be used in pair with `board_resize()`
struct Board *board_create_dummy();
// Creates board, initialises window and panel with provided size and position
struct Board *board_create(size_t height, size_t width, size_t y, size_t x);
// Destroys board, deletes window and panel
void board_destroy(struct Board *board);

void board_resize(struct Board *board, size_t height, size_t width, size_t y, size_t x);
void board_set_color_pair(struct Board *board, int color_pair);

PANEL  *board_panel(const struct Board *board);
WINDOW *board_window(const struct Board *board);
