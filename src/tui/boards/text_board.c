#include "tui/boards/text_board.h"

#include <ncurses.h>
#include <stddef.h>

#include "collections/gap_buffer.h"
#include "core/context.h"
#include "tui/boards/board.h"
#include "tui/color.h"
#include "tui/coords.h"
#include "tui/highlight.h"
#include "tui/text.h" // TODO: once next_valid_coords() moves, change it
#include "tui/tui.h"

void text_board_highlight_line(struct Board *text_board, size_t y) {
    highlight_line(text_board, y, COLOR_PAIR_TEXT_HIGHLIGHTED);
}

// TODO: improve arguments?
void print_gap_buffer_to_board(
    struct Board           *board,
    const struct GapBuffer *gb,
    size_t                  starting_index,
    size_t                  max_rows,
    size_t                  max_columns
) {
    struct Coords current = {.y = 0, .x = 0};

    for (size_t i = starting_index; i < gap_buffer_get_length(gb); i++) {
        char sym = gap_buffer_get_at(gb, i);

        mvwaddch(board_window(board), current.y, current.x, sym);

        if (sym == '\n') {
            print_filler_till_end_of_row(board, &current);
        }

        optional_coords maybe_next = next_valid_coords(&current, max_rows, max_columns, sym);
        if (coords_is_none(maybe_next)) {
            break;
        }
        current = coords_get_val(maybe_next);
    }

    print_filler_till_end_of_board(board, &current);
}

void update_text_board(struct Context *ctx) {
    struct Board     *text_board = ctx->text_board;
    struct GapBuffer *gb = ctx->gap_buffer;
    struct Coords    *cursor = &ctx->cursor;

    print_gap_buffer_to_board(text_board, gb, ctx->starting_symbol_index, text_board->height, text_board->width);

    text_board_highlight_line(text_board, cursor->y);
    highlight_cursor(text_board, cursor->y, cursor->x);
}
