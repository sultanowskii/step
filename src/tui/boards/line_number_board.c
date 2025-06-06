#include "tui/boards/line_number_board.h"

#include <ncurses.h>
#include <stddef.h>

#include "collections/bit_array.h"
#include "collections/gap_buffer.h"
#include "core/context.h"
#include "nonstd/human.h"
#include "tui/boards/board.h"
#include "tui/color.h"
#include "tui/coords.h"
#include "tui/cursor.h"
#include "tui/fmt.h"
#include "tui/highlight.h"
#include "tui/next_coords.h"

static inline void _print_row_line_number(
    struct Board        *line_number_board,
    const struct Coords *current,
    size_t               line_index
) {
    mvwprintw(
        board_window(line_number_board),
        current->y,
        0,
        "%*zu",
        (int)line_number_board->width,
        index_to_human(line_index)
    );
}

static inline void _print_row_blank(
    struct Board        *line_number_board,
    const struct Coords *current
) {
    mvwprintw(
        board_window(line_number_board),
        current->y,
        0,
        "%*c",
        (int)line_number_board->width,
        ' '
    );
}

static inline void _print_row(
    struct Board  *line_number_board,
    struct Coords *text_board_pos,
    size_t         line_index,
    bool           is_newline
) {
    if (is_newline) {
        _print_row_line_number(line_number_board, text_board_pos, line_index);
        return;
    }
    _print_row_blank(line_number_board, text_board_pos);
}

void line_number_board_highlight_line(struct Board *line_number_board, size_t y) {
    highlight_row(line_number_board, y, COLOR_PAIR_LINE_NUMBER_HIGHLIGHTED);
}

// TODO: clean up
void _print_line_number_board_with_gap_buffer(
    struct Context   *ctx,
    struct Board     *line_number_board,
    struct GapBuffer *gb,
    size_t            starting_symbol_index,
    size_t            starting_line_index,
    size_t            text_board_max_rows,
    size_t            text_board_max_columns
) {
    struct Coords text_board_pos = {.y = 0, .x = 0};
    size_t        line_index = starting_line_index;
    size_t        last_handled_row = 0;

    _print_row_line_number(line_number_board, &text_board_pos, line_index);
    last_handled_row = 0;
    line_index++;

    size_t gb_length = gap_buffer_get_length(gb);
    for (size_t i = starting_symbol_index; i < gb_length; i++) {
        char sym = gap_buffer_get_at(gb, i);

        bool x_reached_end_of_row = (text_board_pos.x + 1 == text_board_max_columns);
        bool is_newline = (sym == '\n');

        optional_coords maybe_next = next_valid_coords(&text_board_pos, text_board_max_rows, text_board_max_columns, sym);
        if (coords_is_none(maybe_next)) {
            break;
        }
        text_board_pos = coords_get_val(maybe_next);

        if (x_reached_end_of_row || is_newline) {
            bool row_already_handled = (text_board_pos.y == last_handled_row);
            if (!row_already_handled) {
                bool redraw_required = bit_array_test_at(ctx->rows_to_redraw, text_board_pos.y);
                if (redraw_required) {
                    _print_row(line_number_board, &text_board_pos, line_index, is_newline);
                }
                if (is_newline) {
                    line_index++;
                }
                last_handled_row = text_board_pos.y;
            }
        }
    }

    // fill out the rest of the board
    struct Coords first_unhandled_row_pos = {
        .y = last_handled_row + 1,
        .x = 0,
    };
    print_filler_till_end_of_board(line_number_board, &first_unhandled_row_pos);
}

void update_line_number_board(struct Context *ctx) {
    size_t text_board_max_rows = ctx->text_board->height;
    size_t text_board_max_columns = ctx->text_board->width;

    struct Board     *line_number_board = ctx->line_number_board;
    struct GapBuffer *gb = ctx->gap_buffer;

    _print_line_number_board_with_gap_buffer(
        ctx,
        line_number_board,
        gb,
        ctx->starting_symbol_index,
        ctx->starting_line_index,
        text_board_max_rows,
        text_board_max_columns
    );

    size_t first_row = first_y_of_line_under_cursor(ctx);
    size_t last_row = last_y_of_line_under_cursor(ctx);

    for (size_t y = first_row; y <= last_row; y++) {
        line_number_board_highlight_line(line_number_board, y);
    }
}
