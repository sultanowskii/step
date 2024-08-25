#include "tui/keys/handle_undo_key.h"

#include "core/commands/commands.h"
#include "core/commands/undo.h"
#include "tui/core/context.h"

void handle_undo_key(struct TuiContext *tctx) {
    optional_command_result maybe_result = command_undo(tctx->ctx);
    if (command_result_is_none(maybe_result)) {
        return;
    }
    struct CommandResult *result = command_result_get_val(maybe_result);

    struct EvictingStack *undone = tui_context_get_undone_cmds(tctx);
    struct CommandResult *evicted_result = evicting_stack_push_back(undone, result);
    command_result_destroy(evicted_result);
}

void handle_redo_key(struct TuiContext *tctx) {
    optional_command_result maybe_result = command_redo(tctx->ctx);
    if (command_result_is_none(maybe_result)) {
        return;
    }
    struct CommandResult *result = command_result_get_val(maybe_result);

    struct EvictingStack *done = tui_context_get_done_cmds(tctx);
    struct CommandResult *evicted_result = evicting_stack_push_back(done, result);
    command_result_destroy(evicted_result);
}
