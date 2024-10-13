#include "tui/text.h"

#include <stdbool.h>
#include <stddef.h>

#include "collections/gap_buffer.h"
#include "collections/gap_buffer_str.h"
#include "nonstd/optionals.h"
#include "tui/conf.h"
#include "tui/coords.h"
#include "tui/next_coords.h"
#include "tui/optionals.h"

// TODO: change to work with states (View [no] vs Insert [yes])
size_t gap_buffer_get_max_valid_index(const struct GapBuffer *gb) {
    size_t gb_length = gap_buffer_get_length(gb);
    size_t max_valid_index = ALLOW_CURSOR_AFTER_LAST_SYMBOL ? gb_length : gb_length - 1;
    return max_valid_index;
}

struct Coords revise_coords_with_gap_buffer(
    const struct GapBuffer *gb,
    size_t                  starting_index,
    size_t                  max_rows,
    size_t                  max_columns,
    struct Coords           raw
) {
    struct Coords current = {.y = 0, .x = 0};
    struct Coords last_valid = current;
    size_t        max_valid_index = gap_buffer_get_max_valid_index(gb);

    size_t buffer_index = starting_index;
    while (current.y <= raw.y && buffer_index <= max_valid_index) {
        last_valid = current;

        if (current.y == raw.y && current.x == raw.x) {
            return last_valid;
        }

        char sym = gap_buffer_get_at(gb, buffer_index);

        optional_coords maybe_next = next_valid_coords(&current, max_rows, max_columns, sym);
        if (coords_is_none(maybe_next)) {
            return last_valid;
        }
        current = coords_get_val(maybe_next);
        buffer_index++;
    }

    return last_valid;
}

optional_size_t find_start_of_next_line(
    const struct GapBuffer *gb,
    size_t                  starting_index
) {
    size_t gb_length = gap_buffer_get_length(gb);
    for (size_t i = starting_index; i < gb_length; i++) {
        char symbol = gap_buffer_get_at(gb, i);
        if (symbol == '\n') {
            return size_t_some(i + 1);
        }
    }

    return size_t_none();
}

optional_size_t find_start_of_current_line(
    const struct GapBuffer *gb,
    size_t                  starting_index
) {
    if (starting_index == 0) {
        return size_t_some(0);
    }

    size_t i = starting_index - 1;

    while (true) {
        char symbol = gap_buffer_get_at(gb, i);
        if (symbol == '\n') {
            return size_t_some(i + 1);
        }

        if (i == 0) {
            return size_t_some(0);
        }

        i--;
    }

    return size_t_none();
}

optional_size_t find_start_of_previous_line(
    const struct GapBuffer *gb,
    size_t                  starting_index
) {
    if (starting_index == 0) {
        return size_t_none();
    }

    bool   start_of_current_line_found = false;
    size_t i = starting_index - 1;

    while (true) {
        char symbol = gap_buffer_get_at(gb, i);
        if (symbol == '\n') {
            if (start_of_current_line_found) {
                return size_t_some(i + 1);
            }
            start_of_current_line_found = true;
        }

        if (i == 0) {
            if (start_of_current_line_found) {
                return size_t_some(0);
            }
            break;
        }

        i--;
    }

    return size_t_none();
}

optional_size_t get_index_from_position(
    const struct GapBuffer *gb,
    size_t                  starting_index,
    size_t                  max_rows,
    size_t                  max_columns,
    const struct Coords    *position
) {
    struct Coords current = {.y = 0, .x = 0};
    size_t        max_valid_index = gap_buffer_get_max_valid_index(gb);

    size_t buffer_index = starting_index;
    do {
        if (current.y == position->y && current.x == position->x) {
            return size_t_some(buffer_index);
        }

        char sym = gap_buffer_get_at(gb, buffer_index);

        optional_coords maybe_next = next_valid_coords(&current, max_rows, max_columns, sym);
        if (coords_is_none(maybe_next)) {
            return size_t_none();
        }
        current = coords_get_val(maybe_next);
        buffer_index++;
    } while (buffer_index <= max_valid_index);

    return size_t_none();
}

optional_coords get_position_from_index(
    const struct GapBuffer *gb,
    size_t                  starting_index,
    size_t                  max_rows,
    size_t                  max_columns,
    size_t                  target_index
) {
    struct Coords current = {.y = 0, .x = 0};
    size_t        max_valid_index = gap_buffer_get_max_valid_index(gb);

    size_t buffer_index = starting_index;
    while (buffer_index <= max_valid_index) {
        if (buffer_index == target_index) {
            return coords_some(current);
        }

        char sym = gap_buffer_get_at(gb, buffer_index);

        optional_coords maybe_next = next_valid_coords(&current, max_rows, max_columns, sym);
        if (coords_is_none(maybe_next)) {
            return coords_none();
        }
        current = coords_get_val(maybe_next);
        buffer_index++;
    }

    return coords_none();
}

size_t first_y_of_line_under_pos(
    struct GapBuffer    *gb,
    size_t               starting_symbol_index,
    size_t               text_board_max_rows,
    size_t               text_board_max_columns,
    const struct Coords *pos
) {
    struct Coords text_board_pos = {.y = 0, .x = 0};
    size_t        row_index = 0;

    for (size_t i = starting_symbol_index; i < gap_buffer_get_length(gb); i++) {
        char sym = gap_buffer_get_at(gb, i);

        if (text_board_pos.y == pos->y && text_board_pos.x == pos->x) {
            return row_index;
        }

        optional_coords maybe_next = next_valid_coords(&text_board_pos, text_board_max_rows, text_board_max_columns, sym);
        if (coords_is_none(maybe_next)) {
            break;
        }

        text_board_pos = coords_get_val(maybe_next);

        if (sym == '\n') {
            row_index = text_board_pos.y;
        }
    }

    return row_index;
}

size_t last_y_of_line_under_pos(
    struct GapBuffer    *gb,
    size_t               starting_symbol_index,
    size_t               text_board_max_rows,
    size_t               text_board_max_columns,
    const struct Coords *pos
) {
    struct Coords text_board_pos = {.y = 0, .x = 0};
    size_t        row_index = 0;

    bool after_cursor = false;

    for (size_t i = starting_symbol_index; i < gap_buffer_get_length(gb); i++) {
        char sym = gap_buffer_get_at(gb, i);

        if (text_board_pos.y == pos->y && text_board_pos.x == pos->x) {
            after_cursor = true;
        }

        if (sym == '\n') {
            row_index = text_board_pos.y;
            if (after_cursor) {
                return row_index;
            }
        }

        optional_coords maybe_next = next_valid_coords(&text_board_pos, text_board_max_rows, text_board_max_columns, sym);
        if (coords_is_none(maybe_next)) {
            break;
        }

        text_board_pos = coords_get_val(maybe_next);
    }

    return row_index;
}
