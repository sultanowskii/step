#include "tui/handlers/symbol.h"

#include <stddef.h>

#include "collections/gap_buffer.h"
#include "collections/gap_buffer_str.h"
#include "core/context.h"
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

void handle_newline_added(struct Context *ctx, size_t index) {
    if (is_power_of_10(ctx->line_count)) {
        recompose_boards(ctx);
    }

    if (index < ctx->starting_symbol_index) {
        ctx->starting_line_index++;
    }

    if (ctx->cursor.y == ctx->text_board->height - 1) {
        event_queue_push_request_go_down(ctx->events);
    }
}

void handle_symbol_added(struct Context *ctx, const struct EventSymbolAdded *symbol_added) {
    size_t index = symbol_added->index;

    if (index < ctx->starting_symbol_index) {
        ctx->starting_symbol_index++;
    }

    if (symbol_added->symbol == '\n') {
        return handle_newline_added(ctx, index);
    }
}

void handle_newline_removed(struct Context *ctx, size_t index) {
    // the idea is that we should update in cases like 10->9, 1000->999 and so on.
    // and, as you can notice, 9 + 1 = 10, 99 + 1 = 100, etc.
    if (is_power_of_10(ctx->line_count + 1)) {
        recompose_boards(ctx);
    }

    if (index < ctx->starting_symbol_index) {
        ctx->starting_line_index--;
    }
}

void handle_symbol_removed(struct Context *ctx, const struct EventSymbolRemoved *symbol_removed) {
    size_t index = symbol_removed->index;

    if (symbol_removed->symbol == '\n') {
        return handle_newline_removed(ctx, index);
    }

    if (index < ctx->starting_symbol_index) {
        ctx->starting_symbol_index--;
    }
}
