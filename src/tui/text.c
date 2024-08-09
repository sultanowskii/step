#include "tui/text.h"

#include <stddef.h>

#include "collections/gap_buffer.h"
#include "collections/gap_buffer_str.h"
#include "tui/context.h"
#include "tui/coords.h"

void revise_cursor(struct TuiContext *tctx, size_t max_rows, size_t max_columns) {
    struct GapBuffer *gb = tui_context_get_gap_buffer(tctx);

    struct Coords revised = revise_coords_with_gap_buffer(gb, tctx->buf_starting_symbol_index, max_rows, max_columns, *tctx->cursor);

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
