#include "tui/boards/status_board.h"

#include <ncurses.h>
#include <stddef.h>

#include "human.h"
#include "tui/boards/board.h"
#include "tui/context.h"
#include "tui/coords.h"
#include "tui/optionals.h"
#include "tui/text.h"
#include "tui/tui.h"

void update_status_board(struct TuiContext *tctx, struct Board *text_board, struct Board *status_board) {
    WINDOW *status_board_window = board_window(status_board);

    struct Coords *cursor = tctx->cursor;
    size_t         gb_length = gap_buffer_get_length(tui_context_get_gap_buffer(tctx));

    wclrtoeol(status_board_window);
    mvwprintw(
        status_board_window,
        0,
        0,
        "Cursor position: Ln %zu, Col %zu (y=%zu, x=%zu) len=%zu, i=",
        index_to_human(tctx->starting_line_index + cursor->y),
        index_to_human(cursor->x),
        cursor->y,
        cursor->x,
        gb_length
    );

    optional_size_t maybe_index = get_index_from_cursor_position(tctx, text_board->height, text_board->width);
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

    print_filler_to_eol(status_board, &current);
}
