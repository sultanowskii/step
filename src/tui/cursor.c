#include "tui/cursor.h"

#include <stdbool.h>

#include "core/context.h"
#include "nonstd/optionals.h"
#include "tui/next_coords.h"
#include "tui/optionals.h"
#include "tui/text.h"

void revise_cursor(struct Context *ctx) {
    struct GapBuffer *gb = ctx->gap_buffer;

    struct Coords revised = revise_coords_with_gap_buffer(gb, ctx->starting_symbol_index, ctx->text_board->height, ctx->text_board->width, ctx->cursor);

    ctx->cursor.y = revised.y;
    ctx->cursor.x = revised.x;
}

optional_size_t get_index_from_cursor_position(const struct Context *ctx) {
    struct GapBuffer *gb = ctx->gap_buffer;
    return get_index_from_position(gb, ctx->starting_symbol_index, ctx->text_board->height, ctx->text_board->width, &ctx->cursor);
}

bool _is_index_on_screen(
    const struct GapBuffer *gb,
    size_t                  starting_index,
    size_t                  max_rows,
    size_t                  max_columns,
    size_t                  target_index
) {
    optional_coords maybe_pos = get_position_from_index(
        gb,
        starting_index,
        max_rows,
        max_columns,
        target_index
    );
    return coords_is_some(maybe_pos);
}

bool _is_index_on_real_screen(struct Context *ctx, size_t target_index) {
    return _is_index_on_screen(
        ctx->gap_buffer,
        ctx->starting_symbol_index,
        ctx->text_board->height,
        ctx->text_board->width,
        target_index
    );
}

bool _move_screen_to_index_down(struct Context *ctx, size_t target_index) {
    size_t starting_symbol_index = ctx->starting_symbol_index;
    size_t starting_line_index = ctx->starting_line_index;

    while (target_index < starting_symbol_index) {
        struct FindLineResult res = find_start_of_previous_line(ctx->gap_buffer, starting_symbol_index);
        if (res.found) {
            starting_symbol_index = res.index;
            starting_line_index--;
        } else {
            return false; // TODO: what should happen here?
        }
    }

    ctx->starting_symbol_index = starting_symbol_index;
    ctx->starting_line_index = starting_line_index;
    return true;
}

bool _move_screen_to_index_up(struct Context *ctx, size_t target_index) {
    size_t starting_symbol_index = ctx->starting_symbol_index;
    size_t starting_line_index = ctx->starting_line_index;

    while (!_is_index_on_screen(ctx->gap_buffer, starting_symbol_index, ctx->text_board->height, ctx->text_board->width, target_index)) {
        struct FindLineResult res = find_start_of_next_line(ctx->gap_buffer, starting_symbol_index);
        if (res.found) {
            starting_symbol_index = res.index;
            starting_line_index++;
        } else {
            return false; // TODO: what should happen here?
        }
    }

    ctx->starting_symbol_index = starting_symbol_index;
    ctx->starting_line_index = starting_line_index;
    return true;
}

bool _move_screen_to_index(struct Context *ctx, size_t target_index) {
    if (target_index < ctx->starting_symbol_index) {
        return _move_screen_to_index_down(ctx, target_index);
    } else {
        return _move_screen_to_index_up(ctx, target_index);
    }
}

bool move_cursor_to_index(struct Context *ctx, size_t target_index) {
    if (!_is_index_on_real_screen(ctx, target_index)) {
        if (!_move_screen_to_index(ctx, target_index)) {
            return false;
        }
    }

    optional_coords maybe_pos = get_position_from_index(
        ctx->gap_buffer,
        ctx->starting_symbol_index,
        ctx->text_board->height,
        ctx->text_board->width,
        target_index
    );

    if (coords_is_none(maybe_pos)) {
        return false;
    }

    struct Coords pos = coords_get_val(maybe_pos);
    ctx->cursor = pos;
    return true;
}

optional_size_t get_line_index_from_cursor(const struct Context *ctx) {
    struct GapBuffer   *gb = ctx->gap_buffer;
    const struct Coords cursor = ctx->cursor;
    struct Coords       current = {.y = 0, .x = 0};
    size_t              max_valid_index = gap_buffer_get_max_valid_index(gb);

    size_t buffer_index = ctx->starting_symbol_index;
    size_t line_index = ctx->starting_line_index;

    while (buffer_index <= max_valid_index) {
        if (current.y == cursor.y && current.x == cursor.x) {
            return size_t_some(line_index);
        }

        char sym = gap_buffer_get_at(gb, buffer_index);
        if (sym == '\n') {
            line_index++;
        }

        optional_coords maybe_next = next_valid_coords(&current, ctx->text_board->height, ctx->text_board->width, sym);
        if (coords_is_none(maybe_next)) {
            return size_t_none();
        }
        current = coords_get_val(maybe_next);
        buffer_index++;
    }

    return size_t_none();
}

size_t first_y_of_line_under_cursor(struct Context *ctx) {
    return first_y_of_line_under_pos(
        ctx->gap_buffer,
        ctx->starting_symbol_index,
        ctx->text_board->height,
        ctx->text_board->width,
        &ctx->cursor
    );
}

size_t last_y_of_line_under_cursor(struct Context *ctx) {
    return last_y_of_line_under_pos(
        ctx->gap_buffer,
        ctx->starting_symbol_index,
        ctx->text_board->height,
        ctx->text_board->width,
        &ctx->cursor
    );
}
