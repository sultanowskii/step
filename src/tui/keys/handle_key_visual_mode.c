#include "tui/keys/handle_key_visual_mode.h"

#include "tui/keys/key.h"

void handle_key_visual_mode(struct Context *ctx, int key) {
    struct EventQueue *events = ctx->events;

    switch (key) {
        case ESC:
        case CTRL('q'): {
            bit_array_flood(ctx->rows_to_redraw);
            ctx->state = STATE_NORMAL;
            break;
        }
        case 'c': {
            // TODO: copy
            break;
        }
        case 'x': {
            // TODO: cut
            break;
        }
        case 'd': {
            // TODO: remove
            break;
        }
        case KEY_DOWN:
        case KEY_UP:
        case KEY_LEFT:
        case KEY_RIGHT: {
            event_queue_push_key_navigation(events, key);
            break;
        }
    }
}
