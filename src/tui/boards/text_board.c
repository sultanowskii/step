#include "tui/boards/text_board.h"

#include <ncurses.h>
#include <stddef.h>

#include "collections/gap_buffer.h"
#include "tui/boards/board.h"
#include "tui/color.h"
#include "tui/coords.h"
#include "tui/core/context.h"
#include "tui/highlight.h"
#include "tui/tui.h"

void text_board_highlight_line(struct Board *text_board, size_t y) {
    highlight_line(text_board, y, COLOR_PAIR_TEXT_HIGHLIGHTED);
}

void update_text_board(struct TuiContext *tctx, struct Board *text_board) {
    struct GapBuffer *gb = tui_context_get_gap_buffer(tctx);
    struct Coords    *cursor = tctx->cursor;

    print_gap_buffer_to_board(text_board, gb, tctx->starting_symbol_index, text_board->height, text_board->width);

    text_board_highlight_line(text_board, cursor->y);
    highlight_cursor(text_board, cursor->y, cursor->x);
}

// TODO: improve arguments?
void print_gap_buffer_to_board(
    struct Board           *text_board,
    const struct GapBuffer *gb,
    size_t                  starting_index,
    size_t                  max_rows,
    size_t                  max_columns
) {
    struct Coords current = {.y = 0, .x = 0};

    for (size_t i = starting_index; i < gap_buffer_get_length(gb); i++) {
        char sym = gap_buffer_get_at(gb, i);

        mvwaddch(board_window(text_board), current.y, current.x, sym);

        current.x++;
        if (current.x == max_columns || sym == '\n') {
            if (sym == '\n') {
                print_filler_till_end_of_row(text_board, &current);
            }
            current.y++;
            current.x = 0;
        }
        if (current.y == max_rows) {
            break;
        }
    }

    print_filler_till_end_of_board(text_board, &current);
}
