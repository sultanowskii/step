#include "tui/status_board.h"

#include <ncurses.h>
#include <stddef.h>

#include "human.h"
#include "tui/board.h"
#include "tui/context.h"
#include "tui/coords.h"
#include "tui/tui.h"

void update_status_board(struct TuiContext *tctx, struct Board *status_board) {
    struct Coords *cursor = tctx->cursor;

    wclrtoeol(board_window(status_board));
    mvwprintw(
        board_window(status_board),
        0,
        0,
        "Cursor position: Ln %zu, Col %zu (y=%zu, x=%zu)",
        index_to_human(tctx->buf_starting_line_index + cursor->y),
        index_to_human(cursor->x),
        cursor->y,
        cursor->x
    );

    struct Coords current = {
        .y = getcury(board_window(status_board)),
        .x = getcurx(board_window(status_board)),
    };

    print_filler_to_eol(status_board, &current);
}
