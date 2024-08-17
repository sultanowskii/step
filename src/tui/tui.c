#include "tui/tui.h"

#include <ncurses.h>

#include "tui/boards/board.h"
#include "tui/coords.h"

void print_filler(struct Board *board, const struct Coords *pos, size_t n) {
    mvwprintw(board_window(board), pos->y, pos->x, "%*c", (int)n, ' ');
}

void print_filler_till_end_of_row(struct Board *board, const struct Coords *pos) {
    size_t width = board->width;
    if (pos->x >= width - 1) {
        return;
    }

    size_t n = width - pos->x;

    print_filler(board, pos, n);
}

void print_filler_till_end_of_board(struct Board *board, const struct Coords *pos) {
    print_filler_till_end_of_row(board, pos);

    struct Coords crd = {
        .y = pos->y + 1,
        .x = 0,
    };

    while (crd.y < board->height) {
        print_filler(board, &crd, board->width);
        crd.y++;
    }
}
