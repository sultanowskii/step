#include "tui/handlers/symbol.h"

#include <stddef.h>

#include "collections/gap_buffer.h"
#include "collections/gap_buffer_str.h"
#include "tui/core/context.h"
#include "tui/events/event.h"
#include "tui/events/event_queue.h"
#include "tui/layout.h"

bool is_power_of_10(size_t n) {
    size_t tmp = 1;
    while (tmp < n) {
        tmp *= 10;
    }

    return tmp == n;
}

void handle_newline_added(struct TuiContext *tctx, size_t index) {
    size_t current_line_count = gap_buffer_count_lines(tui_context_get_gap_buffer(tctx));

    if (is_power_of_10(current_line_count)) {
        recompose_boards(tctx->ctx, tctx->line_number_board, tctx->status_board, tctx->text_board);
    }

    if (index < tctx->starting_symbol_index) {
        tctx->starting_line_index++;
    }

    if (tctx->cursor->y == tctx->text_board->height - 1) {
        event_queue_push_request_go_down(tctx->events);
    }
}

void handle_symbol_added(struct TuiContext *tctx, const struct EventSymbolAdded *symbol_added) {
    size_t index = symbol_added->index;

    if (index < tctx->starting_symbol_index) {
        tctx->starting_symbol_index++;
    }

    if (symbol_added->symbol == '\n') {
        return handle_newline_added(tctx, index);
    }
}

void handle_newline_removed(struct TuiContext *tctx, size_t index) {
    size_t current_line_count = gap_buffer_count_lines(tui_context_get_gap_buffer(tctx));

    // the idea is that we should update in cases like 10->9, 1000->999 and so on.
    // and, as you can notice, 9 + 1 = 10, 99 + 1 = 100, etc.
    if (is_power_of_10(current_line_count + 1)) {
        recompose_boards(tctx->ctx, tctx->line_number_board, tctx->status_board, tctx->text_board);
    }

    if (index < tctx->starting_symbol_index) {
        tctx->starting_line_index--;
    }
}

void handle_symbol_removed(struct TuiContext *tctx, const struct EventSymbolRemoved *symbol_removed) {
    size_t index = symbol_removed->index;

    if (symbol_removed->symbol == '\n') {
        return handle_newline_removed(tctx, index);
    }

    if (index < tctx->starting_symbol_index) {
        tctx->starting_symbol_index--;
    }
}
