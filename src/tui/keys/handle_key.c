#include "tui/keys/handle_key.h"

#include <errno.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>

#include "collections/gap_buffer.h"
#include "tui/context.h"
#include "tui/keys/handle_delete_key.h"
#include "tui/keys/handle_navigation_key.h"
#include "tui/keys/handle_other_key.h"
#include "tui/keys/handle_undo_key.h"
#include "tui/keys/key.h"
#include "tui/layout.h"
#include "tui/optionals.h"
#include "tui/text.h"

void handle_key(
    struct TuiContext *tctx,
    struct Board      *line_number_board,
    struct Board      *status_board,
    struct Board      *text_board,
    int                key
) {
    // TODO: handle in separate functions
    switch (key) {
        case KEY_RESIZE: {
            recompose_boards(tctx->ctx, line_number_board, status_board, text_board);
            break;
        }
        case CTRL('s'): {
            const char *filepath = tui_context_get_filepath(tctx);
            FILE       *file = fopen(filepath, "w+");
            gap_buffer_write_to_file(tui_context_get_gap_buffer(tctx), file);
            fclose(file);
            break;
        }
        case CTRL('z'): {
            handle_undo_key(tctx);
            break;
        }
        case CTRL('y'): {
            handle_redo_key(tctx);
            break;
        }
        case CTRL('q'): {
            context_set_state(tctx->ctx, STATE_EXIT);
            break;
        }
        case KEY_DOWN:
        case KEY_UP:
        case KEY_LEFT:
        case KEY_RIGHT: {
            handle_navigation_key(tctx, text_board, key);
            break;
        }
        case KEY_DC:
        case KEY_BACKSPACE: {
            handle_delete_key(tctx, text_board, key);
            break;
        }
        default: {
            handle_other(tctx, text_board, key);
            break;
        }
    }
}
