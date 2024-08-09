#include "tui/text_board.h"

#include <ncurses.h>
#include <stddef.h>

#include "collections/gap_buffer.h"
#include "tui/board.h"
#include "tui/coords.h"

void _print_filler(struct Board *text_board, const struct Coords *pos, size_t n) {
    mvwprintw(board_window(text_board), pos->y, pos->x, "%*c", (int)n, ' ');
}

void print_gap_buffer_to_board(
    const struct GapBuffer *gb,
    struct Board           *text_board,
    size_t                  starting_index,
    size_t                  max_rows,
    size_t                  max_columns
) {
    struct Coords current = {.y = 0, .x = 0};

    size_t gb_length = gap_buffer_get_length(gb);
    for (size_t i = starting_index; i < gb_length; i++) {
        char sym = gap_buffer_get_at(gb, i);

        mvwaddch(board_window(text_board), current.y, current.x, sym);

        current.x++;
        if (current.x == max_columns || sym == '\n') {
            if (sym == '\n') {
                _print_filler(text_board, &current, max_columns - current.x);
            }
            current.y++;
            current.x = 0;
        }
        if (current.y == max_rows) {
            return;
        }
    }

    _print_filler(text_board, &current, max_columns - current.x);
}
