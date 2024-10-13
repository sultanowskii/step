#include "tui/navigation.h"

#include <stdbool.h>

#include "core/context.h"
#include "tui/text.h"

bool try_go_up(struct Context *ctx) {
    optional_size_t maybe_previous_line_start_index = find_start_of_previous_line(ctx->gap_buffer, ctx->starting_symbol_index);
    if (size_t_is_none(maybe_previous_line_start_index)) {
        return false;
    }
    ctx->starting_symbol_index = size_t_get_val(maybe_previous_line_start_index);
    ctx->starting_line_index--;
    return true;
}

bool try_go_down(struct Context *ctx) {
    optional_size_t maybe_next_line_start_index = find_start_of_next_line(ctx->gap_buffer, ctx->starting_symbol_index);
    if (size_t_is_none(maybe_next_line_start_index)) {
        return false;
    }
    ctx->starting_symbol_index = size_t_get_val(maybe_next_line_start_index);
    ctx->starting_line_index++;
    return true;
}
