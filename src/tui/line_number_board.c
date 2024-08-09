#include "tui/line_number_board.h"

#include <ncurses.h>
#include <stddef.h>

#include "collections/gap_buffer.h"
#include "human.h"
#include "tui/board.h"
#include "tui/coords.h"

static inline void _print_line_number(
    struct Board        *line_number_board,
    const struct Coords *current,
    size_t              *line_index
) {
    mvwprintw(
        board_window(line_number_board),
        current->y,
        0,
        "%*zu",
        (int)line_number_board->width,
        index_to_human(*line_index)
    );
    (*line_index)++;
}

static inline void _print_line_number_blank(
    struct Board        *line_number_board,
    const struct Coords *current
) {
    mvwprintw(
        board_window(line_number_board),
        current->y,
        0,
        "%*c",
        (int)line_number_board->width,
        ' '
    );
}

void print_line_numbers_to_board(
    const struct GapBuffer *gb,
    struct Board           *line_number_board,
    size_t                  starting_index,
    size_t                  max_rows,
    size_t                  max_columns,
    size_t                  starting_line_index
) {
    struct Coords current = {.y = 0, .x = 0};
    size_t        line_index = starting_line_index;

    _print_line_number(line_number_board, &current, &line_index);

    size_t gb_length = gap_buffer_get_length(gb);
    for (size_t i = starting_index; i < gb_length; i++) {
        char sym = gap_buffer_get_at(gb, i);

        current.x++;
        if (current.x == max_columns || sym == '\n') {
            current.y++;
            current.x = 0;
            if (sym == '\n') {
                _print_line_number(line_number_board, &current, &line_index);
            } else {
                _print_line_number_blank(line_number_board, &current);
            }
        }
        if (current.y == max_rows) {
            return;
        }
    }
}
