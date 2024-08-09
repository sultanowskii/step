#include "tui/text_board.h"

#include <ncurses.h>
#include <stddef.h>

#include "collections/gap_buffer.h"
#include "tui/board.h"
#include "tui/coords.h"
#include "tui/highlight.h"
#include "tui/tui.h"

// TODO: improve arguments
void update_text_board(struct Board *text_board, const struct GapBuffer *gb, size_t starting_index, const struct Coords *cursor) {
    print_gap_buffer_to_board(text_board, gb, starting_index, text_board->height, text_board->width);

    highlight_on(text_board, cursor->y, cursor->x);
}

// TODO: improve arguments
void print_gap_buffer_to_board(
    struct Board           *text_board,
    const struct GapBuffer *gb,
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
                print_filler_to_eol(text_board, &current);
            }
            current.y++;
            current.x = 0;
        }
        if (current.y == max_rows) {
            return;
        }
    }

    print_filler_to_eol(text_board, &current);
}
