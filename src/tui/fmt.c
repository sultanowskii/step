#include "tui/fmt.h"

#include <ncurses.h>

#include "tui/boards/board.h"
#include "tui/coords.h"

void print_filler(struct Board *board, size_t n) {
    if (n == 0) {
        return;
    }
    wprintw(board_window(board), "%*c", (int)n, ' ');
}

void print_filler_at(struct Board *board, size_t n, const struct Coords *pos) {
    if (n == 0) {
        return;
    }
    mvwprintw(board_window(board), pos->y, pos->x, "%*c", (int)n, ' ');
}

void print_filler_till_end_of_row(struct Board *board, const struct Coords *pos) {
    size_t width = board->width;
    if (pos->x >= width - 1) {
        return;
    }

    size_t n = width - pos->x;

    print_filler_at(board, n, pos);
}

void print_filler_till_end_of_board(struct Board *board, const struct Coords *pos) {
    print_filler_till_end_of_row(board, pos);

    struct Coords crd = {
        .y = pos->y + 1,
        .x = 0,
    };

    while (crd.y < board->height) {
        print_filler_at(board, board->width, &crd);
        crd.y++;
    }
}
