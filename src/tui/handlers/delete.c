#include "tui/handlers/delete.h"

#include <stddef.h>

#include "collections/gap_buffer.h"
#include "core/context.h"
#include "tui/events/event.h"
#include "tui/optionals.h"
#include "tui/text.h"

void handle_deletion(struct Context *ctx, bool backwards) {
    const struct Board *text_board = ctx->text_board;

    optional_size_t maybe_index = get_index_from_cursor_position(ctx, text_board->height, text_board->width);
    if (size_t_is_none(maybe_index)) {
        return;
    }

    struct GapBuffer *gb = ctx->gap_buffer;
    size_t            index = size_t_get_val(maybe_index);

    if (backwards) {
        if (index == 0) {
            return;
        }
        if (ctx->cursor.y == 0 && ctx->cursor.x == 0) {
            if (try_go_up(ctx)) {
                ctx->cursor.y++;
            }
        }
        index--;
    } else {
        size_t gb_length = gap_buffer_get_length(gb);
        if (index >= gb_length) {
            return;
        }
    }

    size_t index_of_deleted_symbol = index;

    char symbol = gap_buffer_get_at(gb, index);

    struct Command       *cmd = command_create_delete_symbol(index);
    struct CommandResult *result = command_exec(ctx, cmd);
    command_destroy(cmd);
    undo_facade_add_done(ctx->undo_facade, result);

    move_cursor_to_index(ctx, text_board->height, text_board->width, index);

    event_queue_push_symbol_removed(ctx->events, index_of_deleted_symbol, symbol);
}

void handle_key_delete(struct Context *ctx, const struct EventKeyDelete *key_delete) {
    handle_deletion(ctx, false);
}

void handle_key_backspace(struct Context *ctx, const struct EventKeyBackspace *key_backspace) {
    handle_deletion(ctx, true);
}
