#include "tui/keys/handle_key_normal_mode.h"

#include <ncurses.h>

#include "core/context.h"
#include "nonstd/optionals.h"
#include "tui/cursor.h"
#include "tui/keys/key.h"
#include "tui/layout.h"

void handle_key_normal_mode(struct Context *ctx, int key) {
    struct EventQueue *events = ctx->events;

    switch (key) {
        case KEY_RESIZE: {
            recompose_boards(ctx);
            break;
        }
        case CTRL('s'): {
            const char *filepath = ctx->filepath;
            FILE       *file = fopen(filepath, "w+");
            gap_buffer_write_to_file(ctx->gap_buffer, file);
            fclose(file);
            break;
        }
        case 'i': {
            ctx->state = STATE_INSERT;
            break;
        }
        case 'v': {
            optional_size_t maybe_current_index = get_index_from_cursor_position(ctx);
            if (size_t_is_some(maybe_current_index)) {
                size_t current_index = size_t_get_val(maybe_current_index);
                ctx->selection_starting_symbol_index = current_index;
                ctx->selection_ending_symbol_index = current_index;
                ctx->state = STATE_VISUAL;
            }
            break;
        }
        case 'u': {
            event_queue_push_key_undo(events);
            break;
        }
        case 'r': {
            event_queue_push_key_redo(events);
            break;
        }
        case 'g': {
            event_queue_push_request_go_to_bof(events);
            break;
        }
        case 'G': {
            event_queue_push_request_go_to_eof(events);
            break;
        }
        case 'p': {
            // TODO: paste
            break;
        }
        case 'x': {
            // TODO: remove one symbol under cursor
            break;
        }
        case CTRL('q'): {
            ctx->state = STATE_EXIT;
            break;
        }
        case KEY_DOWN:
        case KEY_UP:
        case KEY_LEFT:
        case KEY_RIGHT: {
            event_queue_push_key_navigation(events, key);
            break;
        }
        default: {
            break;
        }
    }
}
