#include "tui/handlers/newline.h"

#include <stddef.h>

#include "collections/gap_buffer.h"
#include "collections/gap_buffer_str.h"
#include "tui/core/context.h"
#include "tui/events/event.h"
#include "tui/layout.h"

bool is_power_of_10(size_t n) {
    size_t tmp = 1;
    while (tmp < n) {
        tmp *= 10;
    }

    return tmp == n;
}

void handle_newline_added(struct TuiContext *tctx, const struct EventNewlineAdded *newline_added) {
    size_t current_line_count = gap_buffer_count_lines(tui_context_get_gap_buffer(tctx));

    if (is_power_of_10(current_line_count)) {
        recompose_boards(tctx->ctx, tctx->line_number_board, tctx->status_board, tctx->text_board);
    }

    if (tctx->cursor->y - 1 == tctx->text_board->height) {
        // TODO: ask to move lower
    }
}

void handle_newline_removed(struct TuiContext *tctx, const struct EventNewlineRemoved *newline_removed) {
    size_t current_line_count = gap_buffer_count_lines(tui_context_get_gap_buffer(tctx));

    // the idea is that we should update in cases like 10->9, 1000->999 and so on.
    // and, as you can notice, 9 + 1 = 10, 99 + 1 = 100, etc.
    if (is_power_of_10(current_line_count + 1)) {
        recompose_boards(tctx->ctx, tctx->line_number_board, tctx->status_board, tctx->text_board);
    }

    if (tctx->cursor->y == 0) {
        // TODO: ask to move higher
    }
}
