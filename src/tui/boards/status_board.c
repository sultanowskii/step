#include "tui/boards/status_board.h"

#include <ncurses.h>
#include <stddef.h>

#include "core/context.h"
#include "nonstd/human.h"
#include "tui/boards/board.h"
#include "tui/coords.h"
#include "tui/optionals.h"
#include "tui/text.h"
#include "tui/tui.h"

void update_status_board(struct Context *ctx) {
    struct Board *status_board = ctx->status_board;
    WINDOW       *status_board_window = board_window(status_board);

    struct Coords *cursor = &ctx->cursor;
    size_t         gb_length = gap_buffer_get_length(ctx->gap_buffer);

    optional_size_t maybe_line_index = get_line_index_from_cursor(ctx);
    size_t          line_index;
    if (size_t_is_some(maybe_line_index)) {
        line_index = size_t_get_val(maybe_line_index);
    } else {
        line_index = -1;
    }

    wclrtoeol(status_board_window);
    mvwprintw(
        status_board_window,
        0,
        0,
        "Ln %zu, Col %zu (y=%zu, x=%zu) len=%zu st_i=%zu st_l=%zu ln i=",
        index_to_human(line_index),
        index_to_human(cursor->x), // TODO: long lines
        cursor->y,
        cursor->x,
        gb_length,
        ctx->starting_symbol_index,
        ctx->starting_line_index
    );

    optional_size_t maybe_index = get_index_from_cursor_position(ctx);
    if (size_t_is_some(maybe_index)) {
        size_t index = size_t_get_val(maybe_index);
        wprintw(status_board_window, "%zu", index);
    } else {
        wprintw(status_board_window, "%s", "cant get");
    }

    struct Coords current = {
        .y = getcury(status_board_window),
        .x = getcurx(status_board_window),
    };

    print_filler_till_end_of_board(status_board, &current);
}
