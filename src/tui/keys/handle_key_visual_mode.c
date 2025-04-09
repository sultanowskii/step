#include "tui/keys/handle_key_visual_mode.h"

#include <ncurses.h>

#include "core/state.h"
#include "nonstd/math.h"
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
            size_t index = MIN(ctx->selection_starting_symbol_index, ctx->selection_ending_symbol_index);
            size_t length = UNSIGNED_ABS_DIFF(ctx->selection_starting_symbol_index, ctx->selection_ending_symbol_index) + 1;
            event_queue_push_key_copy(events, index, length);
            ctx->state = STATE_NORMAL;
            break;
        }
        case 'x': {
            size_t index = MIN(ctx->selection_starting_symbol_index, ctx->selection_ending_symbol_index);
            size_t length = UNSIGNED_ABS_DIFF(ctx->selection_starting_symbol_index, ctx->selection_ending_symbol_index) + 1;
            event_queue_push_key_cut(events, index, length);
            ctx->state = STATE_NORMAL;
            break;
        }
        case KEY_DC:
        case KEY_BACKSPACE:
        case 'd': {
            // TODO: delete selected
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
