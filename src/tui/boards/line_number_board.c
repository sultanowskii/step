#include "tui/boards/line_number_board.h"

#include <ncurses.h>
#include <stddef.h>

#include "collections/gap_buffer.h"
#include "human.h"
#include "tui/boards/board.h"
#include "tui/context.h"
#include "tui/coords.h"
#include "tui/tui.h"

static inline void _print_row_line_number(
    struct Board        *line_number_board,
    const struct Coords *current,
    size_t              *line_index
) {
    mvwprintw(
        board_window(line_number_board),
        current->y,
        0,
        "%*zu",
        (int)line_number_board->width,
        index_to_human(*line_index)
    );
    (*line_index)++;
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
    size_t        *line_index,
    bool           is_newline
) {
    if (is_newline) {
        _print_row_line_number(line_number_board, text_board_pos, line_index);
        return;
    }
    _print_row_blank(line_number_board, text_board_pos);
}

void update_line_number_board(
    struct TuiContext *tctx,
    struct Board      *line_number_board,
    size_t             text_board_max_rows,
    size_t             text_board_max_columns
) {
    struct Coords text_board_pos = {.y = 0, .x = 0};
    size_t        line_index = tctx->starting_line_index;
    size_t        last_handled_row = 0;

    _print_row_line_number(line_number_board, &text_board_pos, &line_index);
    last_handled_row = 0;

    struct GapBuffer *gb = tui_context_get_gap_buffer(tctx);

    size_t gb_length = gap_buffer_get_length(gb);
    for (size_t i = tctx->starting_symbol_index; i < gb_length; i++) {
        char sym = gap_buffer_get_at(gb, i);

        text_board_pos.x++;
        bool x_reached_end_of_row = (text_board_pos.x == (text_board_max_columns));
        bool is_newline = (sym == '\n');
        if (x_reached_end_of_row || is_newline) {
            text_board_pos.y++;
            text_board_pos.x = 0;

            bool row_already_handled = (text_board_pos.y == last_handled_row);
            if (!row_already_handled) {
                _print_row(line_number_board, &text_board_pos, &line_index, is_newline);
                last_handled_row = text_board_pos.y;
            }
        }
        if (text_board_pos.y == text_board_max_rows) {
            break;
        }
    }

    // TODO: rename
    struct Coords wild = {
        .y = last_handled_row + 1,
        .x = 0,
    };
    print_filler_till_end_of_board(line_number_board, &wild);
}
