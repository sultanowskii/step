#include "tui/navigation.h"

#include <ncurses.h>

#include "tui/coords.h"
#include "tui/highlight.h"

void handle_navigation_key(
    int            c,
    struct Coords *cursor,
    WINDOW        *text_window,
    size_t         text_window_height,
    size_t         text_window_width
) {
    switch (c) {
    case KEY_RIGHT:
        if (cursor->x == text_window_width - 1) {
            break;
        }
        HIGHLIGHT_OFF(text_window, cursor->y, cursor->x);
        cursor->x++;
        break;
    case KEY_LEFT:
        if (cursor->x == 0) {
            break;
        }
        HIGHLIGHT_OFF(text_window, cursor->y, cursor->x);
        cursor->x--;
        break;
    case KEY_DOWN:
        if (cursor->y == text_window_height - 1) {
            break;
        }
        HIGHLIGHT_OFF(text_window, cursor->y, cursor->x);
        cursor->y++;
        break;
    case KEY_UP:
        if (cursor->y == 0) {
            break;
        }
        HIGHLIGHT_OFF(text_window, cursor->y, cursor->x);
        cursor->y--;
        break;
    default:
        // TODO: handle
    }
}
