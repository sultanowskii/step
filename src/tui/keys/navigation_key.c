#include "tui/keys/navigation_key.h"

#include <ncurses.h>
#include <stdbool.h>

#include "collections/gap_buffer.h"
#include "tui/boards/board.h"
#include "tui/context.h"
#include "tui/coords.h"
#include "tui/highlight.h"
#include "tui/text.h"

enum NavigationRequest handle_navigation_key(
    struct TuiContext *tctx,
    struct Board      *text_board,
    int                c
) {
    struct Coords         *cursor = tctx->cursor;
    enum NavigationRequest requirement = NAVREQ_NO;

    switch (c) {
        case KEY_RIGHT:
            if (cursor->x == text_board->width - 1) {
                break;
            }
            highlight_off(text_board, cursor->y, cursor->x);
            cursor->x++;
            break;
        case KEY_LEFT:
            if (cursor->x == 0) {
                break;
            }
            highlight_off(text_board, cursor->y, cursor->x);
            cursor->x--;
            break;
        case KEY_DOWN:
            if (cursor->y == text_board->height - 1) {
                requirement = NAVREQ_LOWER;
                break;
            }
            highlight_off(text_board, cursor->y, cursor->x);
            cursor->y++;
            break;
        case KEY_UP:
            if (cursor->y == 0) {
                requirement = NAVREQ_UPPER;
                break;
            }
            highlight_off(text_board, cursor->y, cursor->x);
            cursor->y--;
            break;
        default:
            break;
    }

    return requirement;
}

void fulfill_navigation_request(
    struct TuiContext     *tctx,
    enum NavigationRequest request
) {
    struct GapBuffer *gb = tui_context_get_gap_buffer(tctx);

    switch (request) {
        case NAVREQ_UPPER:
            struct FindLineResult find_previous_line_result = find_previous_line(gb, tctx->buf_starting_symbol_index);
            if (find_previous_line_result.found) {
                tctx->buf_starting_symbol_index = find_previous_line_result.index;
                tctx->buf_starting_line_index--;
            }
            break;
        case NAVREQ_LOWER:
            struct FindLineResult find_next_line_result = find_next_line(gb, tctx->buf_starting_symbol_index);
            if (find_next_line_result.found) {
                tctx->buf_starting_symbol_index = find_next_line_result.index;
                tctx->buf_starting_line_index++;
            }
            break;
        case NAVREQ_NO:
        default:
            break;
    }
}
