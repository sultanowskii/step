#include "tui/handlers/navigation.h"

#include <ncurses.h>

#include "collections/gap_buffer.h"
#include "tui/boards/board.h"
#include "tui/coords.h"
#include "tui/core/context.h"
#include "tui/events/event.h"
#include "tui/highlight.h"
#include "tui/navigation.h"
#include "tui/text.h"

void handle_key_navigation(
    struct TuiContext               *tctx,
    const struct EventKeyNavigation *key_navigation
) {
    struct Board  *text_board = tctx->text_board;
    struct Coords *cursor = tctx->cursor;

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
            if (cursor->y == text_board->height - 1) {
                event_queue_push_request_go_down(tctx->events);
                break;
            }
            cursor->y++;
            break;
        case KEY_UP:
            if (cursor->y == 0) {
                event_queue_push_request_go_up(tctx->events);
                break;
            }
            cursor->y--;
            break;
        default: {
            break;
        }
    }
}

void handle_request_go_up(struct TuiContext *tctx, const struct EventGoUpRequest *go_up_request) {
    try_go_up(tctx);
}

void handle_request_go_down(struct TuiContext *tctx, const struct EventGoDownRequest *go_down_request) {
    try_go_down(tctx);
}
