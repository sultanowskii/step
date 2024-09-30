#include "tui/handlers/navigation.h"

#include <ncurses.h>

#include "collections/gap_buffer.h"
#include "core/context.h"
#include "nonstd/compile.h"
#include "tui/boards/board.h"
#include "tui/coords.h"
#include "tui/events/event.h"
#include "tui/highlight.h"
#include "tui/navigation.h"
#include "tui/text.h"

bool row_lower_than_position_exists(const struct Context *ctx) {
    optional_size_t maybe_index = get_index_from_cursor_position(ctx);
    if (size_t_is_none(maybe_index)) {
        return false;
    }

    struct GapBuffer *gb = ctx->gap_buffer;
    size_t            buffer_index = size_t_get_val(maybe_index);
    struct Coords     current = ctx->cursor;
    size_t            max_valid_index = gap_buffer_get_max_valid_index(gb);

    while (buffer_index <= max_valid_index) {
        if (current.y > ctx->cursor.y) {
            return true;
        }

        char sym = gap_buffer_get_at(gb, buffer_index);

        current = next_valid_coords_without_height_limit(&current, ctx->text_board->width, sym);
        buffer_index++;
    }

    return false;
}

void handle_key_navigation(
    struct Context                  *ctx,
    const struct EventKeyNavigation *key_navigation
) {
    struct Board  *text_board = ctx->text_board;
    struct Coords *cursor = &ctx->cursor;

    // TODO: move outta here?
    unhighlight_cursor(text_board, cursor->y, cursor->x);

    switch (key_navigation->key) {
        case KEY_RIGHT:
            if (cursor->x == text_board->width - 1) {
                break;
            }
            cursor->x++;
            break;
        case KEY_LEFT:
            if (cursor->x == 0) {
                break;
            }
            cursor->x--;
            break;
        case KEY_DOWN:
            if (!row_lower_than_position_exists(ctx)) {
                break;
            }

            bool is_cursor_at_bottom_of_screen = cursor->y == text_board->height - 1;
            if (is_cursor_at_bottom_of_screen) {
                event_queue_push_request_go_down(ctx->events);
                break;
            }
            cursor->y++;
            break;
        case KEY_UP:
            if (cursor->y == 0) {
                event_queue_push_request_go_up(ctx->events);
                break;
            }
            cursor->y--;
            break;
        default: {
            break;
        }
    }
}

IGNORE_UNUSED_PARAMETER()
void handle_request_go_up(struct Context *ctx, const struct EventGoUpRequest *go_up_request) {
    try_go_up(ctx);
}

IGNORE_UNUSED_PARAMETER()
void handle_request_go_down(struct Context *ctx, const struct EventGoDownRequest *go_down_request) {
    try_go_down(ctx);
}
