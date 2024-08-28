#include "tui/keys/handle_undo_key.h"

#include <stdbool.h>

#include "core/commands/commands.h"
#include "core/commands/undo.h"
#include "tui/core/context.h"

bool handle_undo_key(struct TuiContext *tctx) {
    struct UndoFacade *undo_facade = tui_context_get_undo_facade(tctx);
    return undo_facade_undo(undo_facade);
}

bool handle_redo_key(struct TuiContext *tctx) {
    struct UndoFacade *undo_facade = tui_context_get_undo_facade(tctx);
    return undo_facade_redo(undo_facade);
}
