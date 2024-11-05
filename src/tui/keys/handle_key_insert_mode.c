#include "tui/keys/handle_key_insert_mode.h"

#include <ncurses.h>

#include "core/context.h"
#include "tui/boards/board.h"
#include "tui/keys/key.h"

void handle_key_insert_mode(
    struct Context *ctx,
    int             key
) {
    struct EventQueue *events = ctx->events;

    switch (key) {
        case KEY_DC: {
            event_queue_push_key_delete(events);
            break;
        }
        case KEY_BACKSPACE: {
            event_queue_push_key_backspace(events);
            break;
        }
        case ESC:
        case CTRL('q'): {
            ctx->state = STATE_NORMAL;
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
            event_queue_push_key_text(events, key);
            break;
        }
    }
}