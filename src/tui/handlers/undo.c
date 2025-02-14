#include "tui/handlers/undo.h"

#include "collections/bit_array.h"
#include "core/commands/undo.h"
#include "core/context.h"
#include "nonstd/compile.h"
#include "tui/cursor.h"
#include "tui/events/event.h"

IGNORE_UNUSED_PARAMETER()
void handle_key_undo(struct Context *ctx, const struct EventKeyUndo *key_undo) {
    struct UndoFacade *undo_facade = ctx->undo_facade;
    optional_size_t    maybe_index = undo_facade_undo(undo_facade);
    if (size_t_is_none(maybe_index)) {
        return;
    }

    size_t index = size_t_get_val(maybe_index);

    move_cursor_to_index(ctx, index);

    bit_array_flood(ctx->rows_to_redraw);
}

IGNORE_UNUSED_PARAMETER()
void handle_key_redo(struct Context *ctx, const struct EventKeyRedo *key_redo) {
    struct UndoFacade *undo_facade = ctx->undo_facade;
    optional_size_t    maybe_index = undo_facade_redo(undo_facade);
    if (size_t_is_none(maybe_index)) {
        return;
    }

    size_t index = size_t_get_val(maybe_index);

    move_cursor_to_index(ctx, index);

    bit_array_flood(ctx->rows_to_redraw);
}
