#include "tui/text.h"

#include <stdbool.h>
#include <stddef.h>

#include "collections/gap_buffer.h"
#include "collections/gap_buffer_str.h"
#include "tui/context.h"
#include "tui/coords.h"
#include "tui/optionals.h"

void revise_cursor(struct TuiContext *tctx, size_t max_rows, size_t max_columns) {
    struct GapBuffer *gb = tui_context_get_gap_buffer(tctx);

    struct Coords revised = revise_coords_with_gap_buffer(gb, tctx->starting_symbol_index, max_rows, max_columns, *tctx->cursor);

    tctx->cursor->y = revised.y;
    tctx->cursor->x = revised.x;
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
        last_valid = current;

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

optional_size_t get_index_from_cursor_position(const struct TuiContext *tctx, size_t max_rows, size_t max_columns) {
    struct GapBuffer *gb = tui_context_get_gap_buffer(tctx);
    return get_index_from_position(gb, tctx->starting_symbol_index, max_rows, max_columns, tctx->cursor);
}

optional_size_t get_index_from_position(
    const struct GapBuffer *gb,
    size_t                  starting_index,
    size_t                  max_rows,
    size_t                  max_columns,
    const struct Coords    *position
) {
    struct Coords current = {.y = 0, .x = 0};
    size_t        gb_length = gap_buffer_get_length(gb);

    size_t buffer_index = starting_index;
    do {
        if (current.y == position->y) {
            if (current.x == position->x) {
                return size_t_some(buffer_index);
            }
        }

        char sym = gap_buffer_get_at(gb, buffer_index);

        current.x++;
        if (current.x == max_columns || sym == '\n') {
            current.x = 0;
            current.y++;
        }
        if (current.y == max_rows) {
            return size_t_none();
        }

        buffer_index++;
    } while (buffer_index < gb_length);

    return size_t_none();
}

bool move_cursor_to_index(const struct TuiContext *tctx, size_t max_rows, size_t max_columns, size_t target_index) {
    struct GapBuffer *gb = tui_context_get_gap_buffer(tctx);
    optional_coords   maybe_pos = get_position_from_index(gb, tctx->starting_symbol_index, max_rows, max_columns, target_index);

    if (coords_is_none(maybe_pos)) {
        return false;
    }

    struct Coords pos = coords_get_val(maybe_pos);
    tctx->cursor->y = pos.y;
    tctx->cursor->x = pos.x;
    return true;
}

optional_coords get_position_from_index(
    const struct GapBuffer *gb,
    size_t                  starting_index,
    size_t                  max_rows,
    size_t                  max_columns,
    size_t                  target_index
) {
    struct Coords current = {.y = 0, .x = 0};
    size_t        gb_length = gap_buffer_get_length(gb);

    size_t buffer_index = starting_index;
    while (buffer_index < gb_length) {
        if (buffer_index == target_index) {
            return coords_some(current);
        }

        char sym = gap_buffer_get_at(gb, buffer_index);

        current.x++;
        if (current.x == max_columns || sym == '\n') {
            current.x = 0;
            current.y++;
        }
        if (current.y == max_rows) {
            return coords_none();
        }

        buffer_index++;
    }

    return coords_none();
}
