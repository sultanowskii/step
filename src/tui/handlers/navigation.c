#include "tui/handlers/navigation.h"

#include <ncurses.h>

#include "collections/gap_buffer.h"
#include "core/context.h"
#include "tui/boards/board.h"
#include "tui/coords.h"
#include "tui/events/event.h"
#include "tui/highlight.h"
#include "tui/navigation.h"
#include "tui/text.h"

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
            bool is_cursor_at_bottom_of_screen = cursor->y == text_board->height - 1;

            optional_size_t maybe_line_index = get_line_index_from_cursor(ctx);
            if (size_t_is_none(maybe_line_index)) {
                break;
            }
            size_t line_index = size_t_get_val(maybe_line_index);
            bool   cursor_at_last_line = line_index == ctx->line_count - 1;

            if (is_cursor_at_bottom_of_screen && !cursor_at_last_line) {
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

void handle_request_go_up(struct Context *ctx, const struct EventGoUpRequest *go_up_request) {
    try_go_up(ctx);
}

void handle_request_go_down(struct Context *ctx, const struct EventGoDownRequest *go_down_request) {
    try_go_down(ctx);
}
