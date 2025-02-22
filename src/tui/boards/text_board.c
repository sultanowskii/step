#include "tui/boards/text_board.h"

#include <ncurses.h>
#include <stddef.h>

#include "collections/bit_array.h"
#include "collections/gap_buffer.h"
#include "core/context.h"
#include "core/state.h"
#include "nonstd/math.h"
#include "tui/boards/board.h"
#include "tui/color.h"
#include "tui/coords.h"
#include "tui/cursor.h"
#include "tui/fmt.h"
#include "tui/highlight.h"
#include "tui/next_coords.h"
#include "tui/optionals.h"
#include "tui/text.h"

void text_board_highlight_line(struct Board *text_board, size_t y) {
    highlight_row(text_board, y, COLOR_PAIR_TEXT_HIGHLIGHTED);
}

void print_gap_buffer_to_board(
    struct Context         *ctx,
    struct Board           *board,
    const struct GapBuffer *gb,
    size_t                  starting_index,
    size_t                  max_rows,
    size_t                  max_columns
) {
    struct Coords current = {.y = 0, .x = 0};

    for (size_t i = starting_index; i < gap_buffer_get_length(gb); i++) {
        char sym = gap_buffer_get_at(gb, i);

        if (bit_array_test_at(ctx->rows_to_redraw, current.y)) {
            mvwaddch(board_window(board), current.y, current.x, sym);
            if (sym == '\n') {
                print_filler_till_end_of_row(board, &current);
            }
        }

        optional_coords maybe_next = next_valid_coords(&current, max_rows, max_columns, sym);
        if (coords_is_none(maybe_next)) {
            break;
        }
        current = coords_get_val(maybe_next);
    }

    print_filler_till_end_of_board(board, &current);
}

void highlight_selected_text(
    struct Context         *ctx,
    struct Board           *board,
    const struct GapBuffer *gb,
    size_t                  starting_index,
    size_t                  max_rows,
    size_t                  max_columns
) {
    size_t        _starting_index = starting_index;
    struct Coords current = (struct Coords){.y = 0, .x = 0};

    size_t selection_start = MIN(ctx->selection_starting_symbol_index, ctx->selection_ending_symbol_index);
    size_t selection_end = MAX(ctx->selection_starting_symbol_index, ctx->selection_ending_symbol_index);

    optional_coords maybe_coords = get_position_from_index(gb, starting_index, max_rows, max_columns, selection_start);
    if (coords_is_some(maybe_coords)) {
        current = coords_get_val(maybe_coords);
        _starting_index = selection_start;
    }

    for (size_t i = _starting_index; i <= MIN(gap_buffer_get_length(gb) - 1, selection_end); i++) {
        char sym = gap_buffer_get_at(gb, i);

        if (selection_start <= i && i <= selection_end) {
            if (bit_array_test_at(ctx->rows_to_redraw, current.y)) {
                highlight_selection(board, current.y, current.x);
            }
        }

        optional_coords maybe_next = next_valid_coords(&current, max_rows, max_columns, sym);
        if (coords_is_none(maybe_next)) {
            break;
        }
        current = coords_get_val(maybe_next);
    }
}

void update_text_board(struct Context *ctx) {
    struct Board     *text_board = ctx->text_board;
    struct GapBuffer *gb = ctx->gap_buffer;
    struct Coords    *cursor = &ctx->cursor;

    print_gap_buffer_to_board(ctx, text_board, gb, ctx->starting_symbol_index, text_board->height, text_board->width);

    size_t first_row = first_y_of_line_under_cursor(ctx);
    size_t last_row = last_y_of_line_under_cursor(ctx);

    for (size_t y = first_row; y <= last_row; y++) {
        text_board_highlight_line(text_board, y);
    }

    if (ctx->state == STATE_VISUAL) {
        highlight_selected_text(ctx, text_board, gb, ctx->starting_symbol_index, text_board->height, text_board->width);
    }

    highlight_cursor(text_board, cursor->y, cursor->x);
}
