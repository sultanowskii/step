#include "tui/navigation.h"

#include <stdbool.h>

#include "tui/core/context.h"
#include "tui/text.h"

bool try_go_up(struct TuiContext *tctx) {
    struct FindLineResult find_previous_line_result = find_start_of_previous_line(tui_context_get_gap_buffer(tctx), tctx->starting_symbol_index);
    if (!find_previous_line_result.found) {
        return false;
    }
    tctx->starting_symbol_index = find_previous_line_result.index;
    tctx->starting_line_index--;
    return true;
}

bool try_go_down(struct TuiContext *tctx) {
    struct FindLineResult find_next_line_result = find_start_of_next_line(tui_context_get_gap_buffer(tctx), tctx->starting_symbol_index);
    if (!find_next_line_result.found) {
        return false;
    }
    tctx->starting_symbol_index = find_next_line_result.index;
    tctx->starting_line_index++;
    return true;
}
