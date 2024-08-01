#include "tui/text.h"

#include <ncurses.h>
#include <stddef.h>

#include "collections/gap_buffer.h"

void gap_buffer_print_to_window(
    struct GapBuffer *gb,
    WINDOW           *win,
    size_t            starting_index,
    size_t            max_rows,
    size_t            max_columns
) {
    size_t y = 0;
    size_t x = 0;

    size_t buffer_index = starting_index;

    while (true) {
        char sym = gap_buffer_get_at(gb, buffer_index);

        if (sym == '\n') {
            x = 0;
            y++;
            goto INDEX_INC;
        }

        mvwaddch(win, y, x, sym);

        x++;
        if (x == max_columns) {
            y++;
            x = 0;
        }
        if (y == max_rows) {
            return;
        }

    INDEX_INC:
        buffer_index++;
    }
}