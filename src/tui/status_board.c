#include "tui/status_board.h"

#include <ncurses.h>
#include <stddef.h>

#include "human.h"
#include "tui/board.h"
#include "tui/coords.h"
#include "tui/tui.h"

void update_status_board(
    struct Board        *status_board,
    size_t               line_index,
    const struct Coords *cursor
) {

    wclrtoeol(board_window(status_board));
    mvwprintw(
        board_window(status_board),
        0,
        0,
        "Cursor position: Ln %zu, Col %zu (y=%zu, x=%zu)",
        index_to_human(line_index + cursor->y),
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
