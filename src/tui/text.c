#include "tui/text.h"

#include <ncurses.h>
#include <stddef.h>

#include "collections/gap_buffer.h"
#include "tui/coords.h"

void gap_buffer_print_to_window(
    const struct GapBuffer *gb,
    WINDOW                 *win,
    size_t                  starting_index,
    size_t                  max_rows,
    size_t                  max_columns
) {
    size_t y = 0;
    size_t x = 0;
    size_t buffer_index = starting_index;

    while (true) {
        char sym = gap_buffer_get_at(gb, buffer_index);

        mvwaddch(win, y, x, sym);

        x++;
        if (x == max_columns || sym == '\n') {
            y++;
            x = 0;
        }
        if (y == max_rows) {
            return;
        }

        buffer_index++;
    }
}

struct Coords gap_buffer_revise_coords(
    const struct GapBuffer *gb,
    size_t                  starting_index,
    size_t                  max_rows,
    size_t                  max_columns,
    size_t                  raw_y,
    size_t                  raw_x
) {
    struct Coords current = {.y = 0, .x = 0};
    size_t        buffer_index = starting_index;

    struct Coords last_valid = {};

    while (current.y <= raw_y) {
        last_valid.y = current.y;
        last_valid.x = current.x;

        if (current.y == raw_y) {
            if (current.x == raw_x) {
                return last_valid;
            }
        }

        char sym = gap_buffer_get_at(gb, buffer_index);

        current.x++;
        if (current.x == max_columns || sym == '\n') {
            current.x = 0;
            current.y++;
        }
        if (current.y == max_rows) {
            return last_valid;
        }

        buffer_index++;
    }

    return last_valid;
}
