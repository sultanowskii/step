#include "tui/navigation.h"

#include <stdbool.h>

#include "core/context.h"
#include "tui/text.h"

bool try_go_up(struct Context *ctx) {
    struct FindLineResult find_previous_line_result = find_start_of_previous_line(ctx->gap_buffer, ctx->starting_symbol_index);
    if (!find_previous_line_result.found) {
        return false;
    }
    ctx->starting_symbol_index = find_previous_line_result.index;
    ctx->starting_line_index--;
    return true;
}

bool try_go_down(struct Context *ctx) {
    struct FindLineResult find_next_line_result = find_start_of_next_line(ctx->gap_buffer, ctx->starting_symbol_index);
    if (!find_next_line_result.found) {
        return false;
    }
    ctx->starting_symbol_index = find_next_line_result.index;
    ctx->starting_line_index++;
    return true;
}
