#include "tui/keys/handle_key_normal_mode.h"

#include <ncurses.h>

#include "core/context.h"
#include "tui/boards/board.h"
#include "tui/keys/key.h"
#include "tui/layout.h"

void handle_key_normal_mode(
    struct Context *ctx,
    int             key
) {
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
        case 'u': {
            event_queue_push_key_undo(events);
            break;
        }
        case 'r': {
            event_queue_push_key_redo(events);
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