#include "tui/text.h"

#include <stdbool.h>
#include <stddef.h>

#include "collections/gap_buffer.h"
#include "collections/gap_buffer_str.h"
#include "nonstd/optionals.h"
#include "tui/conf.h"
#include "tui/coords.h"
#include "tui/optionals.h"

// TODO: change to work with states (View [no] vs Insert [yes])
size_t gap_buffer_get_max_valid_index(const struct GapBuffer *gb) {
    size_t gb_length = gap_buffer_get_length(gb);
    size_t max_valid_index = ALLOW_CURSOR_AFTER_LAST_SYMBOL ? gb_length : gb_length - 1;
    return max_valid_index;
}

// TODO: move to other file
optional_coords next_valid_coords(
    const struct Coords *coords,
    size_t               max_rows,
    size_t               max_columns,
    char                 symbol
) {
    struct Coords next = *coords;

    next.x++;
    if (next.x == max_columns || symbol == '\n') {
        next.x = 0;
        next.y++;
    }
    if (next.y == max_rows) {
        return coords_none();
    }

    return coords_some(next);
}

struct Coords next_valid_coords_without_height_limit(
    const struct Coords *coords,
    size_t               max_columns,
    char                 symbol
) {
    struct Coords next = *coords;

    next.x++;
    if (next.x == max_columns || symbol == '\n') {
        next.x = 0;
        next.y++;
    }

    return next;
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

struct FindLineResult find_start_of_next_line(
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

struct FindLineResult find_start_of_previous_line(
    const struct GapBuffer *gb,
    size_t                  starting_index
) {
    if (starting_index == 0) {
        return (struct FindLineResult){
            .found = false,
        };
    }

    bool   start_of_current_line_found = false;
    size_t i = starting_index - 1;

    while (true) {
        char symbol = gap_buffer_get_at(gb, i);
        if (symbol == '\n') {
            if (start_of_current_line_found) {
                return (struct FindLineResult){
                    .found = true,
                    .index = i + 1,
                };
            } else {
                start_of_current_line_found = true;
            }
        }

        if (i == 0) {
            if (start_of_current_line_found) {
                return (struct FindLineResult){
                    .found = true,
                    .index = 0,
                };
            }
            break;
        }

        i--;
    }

    return (struct FindLineResult){
        .found = false,
    };
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
