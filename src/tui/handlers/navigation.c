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
    struct Board                  *text_board = tctx->text_board;
    struct Coords                 *cursor = tctx->cursor;
    enum SingularNavigationRequest request = SINGULAR_NAVREQ_NO;

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
                request = SINGULAR_NAVREQ_LOWER;
                break;
            }
            cursor->y++;
            break;
        case KEY_UP:
            if (cursor->y == 0) {
                request = SINGULAR_NAVREQ_UPPER;
                break;
            }
            cursor->y--;
            break;
        default: {
            break;
        }
    }

    if (request != SINGULAR_NAVREQ_NO) {
        event_queue_push_singular_navigation_request(tctx->events, request);
    }
}

void handle_singular_navigation_request(
    struct TuiContext                           *tctx,
    const struct EventSingularNavigationRequest *singular_navigation_request
) {
    struct GapBuffer *gb = tui_context_get_gap_buffer(tctx);

    switch (singular_navigation_request->req) {
        case SINGULAR_NAVREQ_UPPER: {
            struct FindLineResult find_previous_line_result = find_previous_line(gb, tctx->starting_symbol_index);
            if (find_previous_line_result.found) {
                tctx->starting_symbol_index = find_previous_line_result.index;
                tctx->starting_line_index--;
            }
            break;
        }
        case SINGULAR_NAVREQ_LOWER: {
            struct FindLineResult find_next_line_result = find_next_line(gb, tctx->starting_symbol_index);
            if (find_next_line_result.found) {
                tctx->starting_symbol_index = find_next_line_result.index;
                tctx->starting_line_index++;
            }
            break;
        }
        case SINGULAR_NAVREQ_NO:
        default: {
            break;
        }
    }
}
