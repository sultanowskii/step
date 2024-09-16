#include "tui/handlers/undo.h"

#include "core/commands/undo.h"
#include "core/context.h"
#include "nonstd/compile.h"
#include "tui/events/event.h"

IGNORE_UNUSED_PARAMETER()
void handle_key_undo(struct Context *ctx, const struct EventKeyUndo *key_undo) {
    struct UndoFacade *undo_facade = ctx->undo_facade;
    undo_facade_undo(undo_facade);
}

IGNORE_UNUSED_PARAMETER()
void handle_key_redo(struct Context *ctx, const struct EventKeyRedo *key_redo) {
    struct UndoFacade *undo_facade = ctx->undo_facade;
    undo_facade_redo(undo_facade);
}
