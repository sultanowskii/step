#include "tui/keys/handle_key.h"

#include <errno.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>

#include "collections/gap_buffer.h"
#include "collections/queue.h"
#include "tui/core/context.h"
#include "tui/events/event.h"
#include "tui/keys/handle_delete_key.h"
#include "tui/keys/handle_navigation_key.h"
#include "tui/keys/handle_other_key.h"
#include "tui/keys/key.h"
#include "tui/layout.h"
#include "tui/optionals.h"
#include "tui/text.h"

void handle_key(
    struct TuiContext *tctx,
    int                key
) {
    struct Board      *line_number_board = tctx->line_number_board;
    struct Board      *status_board = tctx->status_board;
    struct Board      *text_board = tctx->text_board;
    struct EventQueue *events = tctx->events;

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
            event_queue_push_key_undo(events);
            break;
        }
        case CTRL('y'): {
            event_queue_push_key_redo(events);
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
            handle_navigation_key(tctx, key);
            break;
        }
        case KEY_DC:
        case KEY_BACKSPACE: {
            optional_char maybe_deleted_symbol = handle_delete_key(tctx, key);
            // TODO: improve
            if (char_is_some(maybe_deleted_symbol) && char_get_val(maybe_deleted_symbol) == '\n') {
                recompose_boards(tctx->ctx, line_number_board, status_board, text_board);
            }
            break;
        }
        default: {
            optional_char maybe_inserted_symbol = handle_other(tctx, key);
            // TODO: improve
            if (char_is_some(maybe_inserted_symbol) && char_get_val(maybe_inserted_symbol) == '\n') {
                recompose_boards(tctx->ctx, line_number_board, status_board, text_board);
            }
            break;
        }
    }
}
