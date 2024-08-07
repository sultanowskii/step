#include "tui/highlight.h"

#include <ncurses.h>

#include "tui/board.h"

// TODO: color
void highlight_on(struct Board *board, size_t y, size_t x) {
    mvwchgat(board_window(board), y, x, 1, A_REVERSE, 0, NULL);
}

void highlight_off(struct Board *board, size_t y, size_t x) {
    mvwchgat(board_window(board), y, x, 1, A_NORMAL, 0, NULL);
}
