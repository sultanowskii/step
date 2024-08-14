#include "tui/boards/line_number_board.h"

#include <ncurses.h>
#include <stddef.h>

#include "collections/gap_buffer.h"
#include "human.h"
#include "tui/boards/board.h"
#include "tui/context.h"
#include "tui/coords.h"

static inline void _print_line_number(
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

static inline void _print_line_number_blank(
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

void update_line_number_board(
    struct TuiContext *tctx,
    struct Board      *line_number_board,
    size_t             max_rows,
    size_t             max_columns
) {
    struct Coords current = {.y = 0, .x = 0};
    size_t        line_index = tctx->starting_line_index;
    size_t        last_handled_row = 0;

    _print_line_number(line_number_board, &current, &line_index);
    last_handled_row = 0;

    struct GapBuffer *gb = tui_context_get_gap_buffer(tctx);

    size_t gb_length = gap_buffer_get_length(gb);
    for (size_t i = tctx->starting_symbol_index; i < gb_length; i++) {
        char sym = gap_buffer_get_at(gb, i);

        current.x++;
        if (current.x == max_columns || sym == '\n') {
            if (current.y != last_handled_row) {
                if (sym == '\n') {
                    _print_line_number(line_number_board, &current, &line_index);
                } else {
                    _print_line_number_blank(line_number_board, &current);
                }
                last_handled_row = current.y;
            }
            current.y++;
            current.x = 0;
        }
        if (current.y == max_rows) {
            return;
        }
    }

    struct Coords tmp = {
        .y = last_handled_row + 1,
    };
    while (tmp.y < max_rows) {
        _print_line_number_blank(line_number_board, &tmp);
        tmp.y++;
    }
}
