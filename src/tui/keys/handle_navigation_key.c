#include "tui/keys/handle_navigation_key.h"

#include <ncurses.h>
#include <stdbool.h>

#include "collections/gap_buffer.h"
#include "tui/boards/board.h"
#include "tui/coords.h"
#include "tui/core/context.h"
#include "tui/highlight.h"
#include "tui/text.h"

enum NavigationRequest {
    NAVREQ_NO,
    NAVREQ_LOWER,
    NAVREQ_UPPER,
};

void fulfill_navigation_request(
    struct TuiContext     *tctx,
    enum NavigationRequest request
) {
    struct GapBuffer *gb = tui_context_get_gap_buffer(tctx);

    switch (request) {
        case NAVREQ_UPPER: {
            struct FindLineResult find_previous_line_result = find_previous_line(gb, tctx->starting_symbol_index);
            if (find_previous_line_result.found) {
                tctx->starting_symbol_index = find_previous_line_result.index;
                tctx->starting_line_index--;
            }
            break;
        }
        case NAVREQ_LOWER: {
            struct FindLineResult find_next_line_result = find_next_line(gb, tctx->starting_symbol_index);
            if (find_next_line_result.found) {
                tctx->starting_symbol_index = find_next_line_result.index;
                tctx->starting_line_index++;
            }
            break;
        }
        case NAVREQ_NO:
        default: {
            break;
        }
    }
}

void handle_navigation_key(
    struct TuiContext *tctx,
    int                c
) {
    struct Board          *text_board = tctx->text_board;
    struct Coords         *cursor = tctx->cursor;
    enum NavigationRequest request = NAVREQ_NO;

    // TODO: move outta here?
    unhighlight_cursor(text_board, cursor->y, cursor->x);

    switch (c) {
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
                request = NAVREQ_LOWER;
                break;
            }
            cursor->y++;
            break;
        case KEY_UP:
            if (cursor->y == 0) {
                request = NAVREQ_UPPER;
                break;
            }
            cursor->y--;
            break;
        default: {
            break;
        }
    }

    fulfill_navigation_request(tctx, request);
}
