#include "tui/highlight.h"

#include <ncurses.h>

#include "tui/boards/board.h"
#include "tui/color.h"

void highlight_cursor(struct Board *board, size_t y, size_t x) {
    mvwchgat(board_window(board), y, x, 1, A_NORMAL, COLOR_PAIR_TEXT_CURSOR, NULL);
}

void unhighlight_cursor(struct Board *board, size_t y, size_t x) {
    mvwchgat(board_window(board), y, x, 1, A_NORMAL, COLOR_PAIR_TEXT, NULL);
}

void highlight_line(struct Board *board, size_t y, int color_pair) {
    mvwchgat(board_window(board), y, 0, board->width, A_NORMAL, color_pair, NULL);
}
