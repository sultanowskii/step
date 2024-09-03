#include "tui/handlers/undo.h"

#include "core/commands/undo.h"
#include "nonstd/compile.h"
#include "tui/core/context.h"
#include "tui/events/event.h"

IGNORE_UNUSED_PARAMETER()
void handle_key_undo(struct TuiContext *tctx, struct EventKeyUndo *key_undo) {
    struct UndoFacade *undo_facade = tui_context_get_undo_facade(tctx);
    undo_facade_undo(undo_facade);
}

IGNORE_UNUSED_PARAMETER()
void handle_key_redo(struct TuiContext *tctx, struct EventKeyRedo *key_redo) {
    struct UndoFacade *undo_facade = tui_context_get_undo_facade(tctx);
    undo_facade_redo(undo_facade);
}
