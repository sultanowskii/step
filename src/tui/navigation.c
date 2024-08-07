#include "tui/navigation.h"

#include <ncurses.h>

#include "tui/board.h"
#include "tui/coords.h"
#include "tui/highlight.h"

void handle_navigation_key(
    int            c,
    struct Coords *cursor,
    struct Board  *text_board
) {
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
            break;
        }
        highlight_off(text_board, cursor->y, cursor->x);
        cursor->y++;
        break;
    case KEY_UP:
        if (cursor->y == 0) {
            break;
        }
        highlight_off(text_board, cursor->y, cursor->x);
        cursor->y--;
        break;
    default:
        // TODO: handle
    }
}
