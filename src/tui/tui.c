#include "tui/tui.h"

#include <ncurses.h>

#include "tui/board.h"
#include "tui/coords.h"

void print_filler(struct Board *board, const struct Coords *pos, size_t n) {
    mvwprintw(board_window(board), pos->y, pos->x, "%*c", (int)n, ' ');
}

void print_filler_to_eol(struct Board *board, const struct Coords *pos) {
    size_t width = getmaxx(board_window(board));

    size_t n = width - pos->x - 1;

    print_filler(board, pos, n);
}
