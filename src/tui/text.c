#include "tui/text.h"

#include <ncurses.h>
#include <stddef.h>

#include "collections/gap_buffer.h"
#include "human.h"
#include "tui/board.h"
#include "tui/coords.h"

void gap_buffer_print_to_board(
    const struct GapBuffer *gb,
    struct Board           *board,
    size_t                  starting_index,
    size_t                  max_rows,
    size_t                  max_columns
) {
    struct Coords current = {.y = 0, .x = 0};
    size_t        buffer_index = starting_index;

    while (true) {
        char sym = gap_buffer_get_at(gb, buffer_index);

        mvwaddch(board_window(board), current.y, current.x, sym);

        current.x++;
        if (current.x == max_columns || sym == '\n') {
            current.y++;
            current.x = 0;
        }
        if (current.y == max_rows) {
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
    struct Coords           raw
) {
    struct Coords current = {.y = 0, .x = 0};
    size_t        buffer_index = starting_index;

    struct Coords last_valid = {};

    while (current.y <= raw.y) {
        last_valid.y = current.y;
        last_valid.x = current.x;

        if (current.y == raw.y) {
            if (current.x == raw.x) {
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
    size_t        buffer_index = starting_index;
    size_t        line_index = starting_line_index;

    _print_line_number(line_number_board, &current, &line_index);

    while (true) {
        char sym = gap_buffer_get_at(gb, buffer_index);

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

        buffer_index++;
    }
}
