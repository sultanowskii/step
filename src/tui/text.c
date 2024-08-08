#include "tui/text.h"

#include <ncurses.h>
#include <stddef.h>

#include "collections/gap_buffer.h"
#include "collections/gap_buffer_str.h"
#include "human.h"
#include "tui/board.h"
#include "tui/coords.h"

void print_gap_buffer_to_board(
    const struct GapBuffer *gb,
    struct Board           *board,
    size_t                  starting_index,
    size_t                  max_rows,
    size_t                  max_columns
) {
    struct Coords current = {.y = 0, .x = 0};

    size_t gb_length = gap_buffer_get_length(gb);
    for (size_t i = starting_index; i < gb_length; i++) {
        char sym = gap_buffer_get_at(gb, i);

        mvwaddch(board_window(board), current.y, current.x, sym);

        current.x++;
        if (current.x == max_columns || sym == '\n') {
            current.y++;
            current.x = 0;
        }
        if (current.y == max_rows) {
            return;
        }
    }
}

struct Coords revise_coords_with_gap_buffer(
    const struct GapBuffer *gb,
    size_t                  starting_index,
    size_t                  max_rows,
    size_t                  max_columns,
    struct Coords           raw
) {
    struct Coords current = {.y = 0, .x = 0};
    struct Coords last_valid = {};
    size_t        gb_length = gap_buffer_get_length(gb);

    size_t buffer_index = starting_index;
    while (current.y <= raw.y && buffer_index < gb_length) {
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

struct FindLineResult find_next_line(
    const struct GapBuffer *gb,
    size_t                  starting_index
) {
    size_t gb_length = gap_buffer_get_length(gb);
    for (size_t i = starting_index; i < gb_length; i++) {
        char symbol = gap_buffer_get_at(gb, i);
        if (symbol == '\n') {
            return (struct FindLineResult){
                .found = true,
                .index = i + 1,
            };
        }
    }

    return (struct FindLineResult){
        .found = false,
        .index = 0,
    };
}

struct FindLineResult find_previous_line(
    const struct GapBuffer *gb,
    size_t                  starting_index
) {
    if (starting_index < 2) {
        return (struct FindLineResult){
            .found = false,
            .index = 0,
        };
    }

    // we start from [-2] because [0] is the beginning of current line and [-1] is \n
    for (size_t i = starting_index - 2; i != 0; i--) {
        char symbol = gap_buffer_get_at(gb, i);
        if (symbol == '\n') {
            return (struct FindLineResult){
                .found = true,
                .index = i + 1,
            };
        }
    }

    return (struct FindLineResult){
        .found = true,
        .index = 0,
    };
}