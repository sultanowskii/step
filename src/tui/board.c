#include "tui/board.h"

#include <malloc.h>
#include <ncurses.h>
#include <panel.h>
#include <stddef.h>

struct Board *board_create_empty() {
    struct Board *board = malloc(sizeof(struct Board));
    board->panel = NULL;
    board->height = 0;
    board->width = 0;
    board->start = (struct Coords){
        .y = 0,
        .x = 0,
    };
    return board;
}

struct Board *board_create(
    size_t height,
    size_t width,
    size_t y,
    size_t x
) {
    struct Board *board = board_create_empty();
    WINDOW       *win = newwin(height, width, y, x);
    PANEL        *panel = new_panel(win);
    board->panel = panel;
    board->height = height;
    board->width = width;
    board->start = (struct Coords){
        .y = y,
        .x = x,
    };
    return board;
}

struct Board *board_create_dummy() {
    struct Board *board = board_create(1, 1, 0, 0);
    return board;
}

void board_destroy(struct Board *board) {
    WINDOW *win = panel_window(board->panel);
    del_panel(board->panel);
    delwin(win);
    free(board);
}

void board_resize(struct Board *board, size_t height, size_t width, size_t y, size_t x) {
    WINDOW *old_window = board_window(board);

    WINDOW *new_window = newwin(height, width, y, x);
    replace_panel(board_panel(board), new_window);

    board->height = height;
    board->width = width;
    board->start.y = y;
    board->start.x = x;

    if (old_window != NULL) {
        delwin(old_window);
    }
}

PANEL *board_panel(const struct Board *board) {
    return board->panel;
}

WINDOW *board_window(const struct Board *board) {
    return panel_window(board_panel(board));
}
