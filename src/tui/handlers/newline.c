#include "tui/handlers/newline.h"

#include <stddef.h>

#include "collections/gap_buffer.h"
#include "collections/gap_buffer_str.h"
#include "tui/core/context.h"
#include "tui/events/event.h"
#include "tui/layout.h"

void handle_newline_added(struct TuiContext *tctx, struct EventNewlineAdded *newline_added) {
    size_t prev_line_count_digits = count_digits(newline_added->prev_line_count);

    size_t current_line_count = gap_buffer_count_lines(tui_context_get_gap_buffer(tctx));
    size_t current_line_count_digits = count_digits(current_line_count);

    if (current_line_count_digits != prev_line_count_digits) {
        recompose_boards(tctx->ctx, tctx->line_number_board, tctx->status_board, tctx->text_board);
    }
}

void handle_newline_removed(struct TuiContext *tctx, struct EventNewlineRemoved *newline_removed) {
    size_t prev_line_count_digits = count_digits(newline_removed->prev_line_count);

    size_t current_line_count = gap_buffer_count_lines(tui_context_get_gap_buffer(tctx));
    size_t current_line_count_digits = count_digits(current_line_count);

    if (current_line_count_digits != prev_line_count_digits) {
        recompose_boards(tctx->ctx, tctx->line_number_board, tctx->status_board, tctx->text_board);
    }
}