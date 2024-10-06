#include "tui/keys/handle_key.h"

#include <errno.h>
#include <ncurses.h>
#include <stdbool.h>

#include "collections/gap_buffer.h"
#include "collections/queue.h"
#include "core/context.h"
#include "tui/events/event.h"
#include "tui/keys/key.h"
#include "tui/layout.h"

#include "tui/cursor.h"

void handle_key(
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
        case CTRL('z'): {
            event_queue_push_key_undo(events);
            break;
        }
        case CTRL('y'): {
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
        case KEY_DC:
            event_queue_push_key_delete(events);
            break;
        case KEY_BACKSPACE: {
            event_queue_push_key_backspace(events);
            break;
        }
        default: {
            event_queue_push_key_text(events, key);
            break;
        }
    }
}
