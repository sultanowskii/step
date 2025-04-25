#include "tui/keys/handle_key_visual_mode.h"

#include <ncurses.h>

#include "core/state.h"
#include "nonstd/math.h"
#include "tui/keys/key.h"

void handle_key_visual_mode(struct Context *ctx, int key) {
    struct EventQueue *events = ctx->events;

    size_t selection_index = MIN(ctx->selection_starting_symbol_index, ctx->selection_ending_symbol_index);
    size_t selection_length = UNSIGNED_ABS_DIFF(ctx->selection_starting_symbol_index, ctx->selection_ending_symbol_index) + 1;

    switch (key) {
        case ESC:
        case CTRL('q'): {
            bit_array_flood(ctx->rows_to_redraw);
            ctx->state = STATE_NORMAL;
            break;
        }
        case 'c': {
            event_queue_push_key_copy(events, selection_index, selection_length);
            ctx->state = STATE_NORMAL;
            break;
        }
        case 'x': {
            event_queue_push_key_cut(events, selection_index, selection_length);
            ctx->state = STATE_NORMAL;
            break;
        }
        case KEY_DC:
        case KEY_BACKSPACE:
        case 'd': {
            event_queue_push_request_delete_string(events, selection_index, selection_length);
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
    }
}
