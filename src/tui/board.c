#include "tui/board.h"

#include <ncurses.h>
#include <panel.h>
#include <stddef.h>

struct Board create_board(
    size_t height,
    size_t width,
    size_t offset_y,
    size_t offset_x
) {
    WINDOW *win = newwin(height, width, offset_y, offset_x);
    PANEL  *panel = new_panel(win);
    return (struct Board){
        .panel = panel,
        .height = height,
        .width = width,
        .start = {
            .y = offset_y,
            .x = offset_x,
        },
    };
}

void board_destroy(struct Board *board) {
    WINDOW *win = panel_window(board->panel);
    del_panel(board->panel);
    delwin(win);
}

PANEL *board_panel(const struct Board *board) {
    return board->panel;
}

WINDOW *board_window(const struct Board *board) {
    return panel_window(board_panel(board));
}
