#include "tui/keys/handle_key.h"

#include <errno.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>

#include "collections/gap_buffer.h"
#include "io.h"
#include "tui/context.h"
#include "tui/keys/handle_delete_key.h"
#include "tui/keys/handle_navigation_key.h"
#include "tui/keys/handle_other_key.h"
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
    switch (key) {
        case KEY_RESIZE: {
            recreate_boards(tctx->ctx, line_number_board, status_board, text_board);
            break;
        }
        case CTRL('s'): {
            const char *filepath = tui_context_get_filepath(tctx);
            FILE       *file = fopen(filepath, "w+");
            gap_buffer_write_to_file(tui_context_get_gap_buffer(tctx), file);
            fclose(file);
            break;
        }
        case KEY_DOWN:
        case KEY_UP:
        case KEY_LEFT:
        case KEY_RIGHT: {
            enum NavigationRequest request = handle_navigation_key(tctx, text_board, key);
            if (request != NAVREQ_NO) {
                fulfill_navigation_request(tctx, request);
            }
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
