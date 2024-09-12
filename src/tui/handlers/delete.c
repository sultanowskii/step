#include "tui/handlers/delete.h"

#include <stddef.h>

#include "collections/gap_buffer.h"
#include "tui/core/context.h"
#include "tui/events/event.h"
#include "tui/optionals.h"
#include "tui/text.h"

void handle_deletion(struct TuiContext *tctx, bool backwards) {
    const struct Board *text_board = tctx->text_board;

    optional_size_t maybe_index = get_index_from_cursor_position(tctx, text_board->height, text_board->width);
    if (size_t_is_none(maybe_index)) {
        return;
    }

    struct GapBuffer *gb = tui_context_get_gap_buffer(tctx);
    size_t            index = size_t_get_val(maybe_index);

    if (backwards) {
        if (index == 0) {
            return;
        }
        index--;

    } else {
        size_t gb_length = gap_buffer_get_length(gb);
        if (index >= gb_length) {
            return;
        }
    }

    char symbol = gap_buffer_get_at(gb, index);

    struct Command       *cmd = command_create_delete_symbol(index);
    struct CommandResult *result = command_exec(tctx->ctx, cmd);
    command_destroy(cmd);
    undo_facade_add_done(tui_context_get_undo_facade(tctx), result);

    move_cursor_to_index(tctx, text_board->height, text_board->width, index);

    if (symbol == '\n') {
        event_queue_push_newline_removed(tctx->events);
    }
}

void handle_key_delete(struct TuiContext *tctx, const struct EventKeyDelete *key_delete) {
    handle_deletion(tctx, false);
}

void handle_key_backspace(struct TuiContext *tctx, const struct EventKeyBackspace *key_backspace) {
    handle_deletion(tctx, true);
}
