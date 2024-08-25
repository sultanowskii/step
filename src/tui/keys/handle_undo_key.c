#include "tui/keys/handle_undo_key.h"

#include "core/commands.h"
#include "tui/core/context.h"

void handle_undo_key(struct TuiContext *tctx) {
    struct EvictingStack *done = tui_context_get_done_cmds(tctx);
    struct EvictingStack *undone = tui_context_get_undone_cmds(tctx);
    if (evicting_stack_get_size(done) == 0) {
        return;
    }

    // TODO: extract it all into separate function
    struct CommandResult *popped_result = evicting_stack_pop_back(done);
    struct Command       *inverse_command = command_create_from_opposing_result(popped_result);
    command_result_destroy(popped_result);
    struct CommandResult *result = command_exec(tctx->ctx, inverse_command);
    command_destroy(inverse_command);
    struct CommandResult *evicted_result = evicting_stack_push_back(undone, result);
    command_result_destroy(evicted_result);
}

void handle_redo_key(struct TuiContext *tctx) {
    struct EvictingStack *undone = tui_context_get_undone_cmds(tctx);
    struct EvictingStack *done = tui_context_get_done_cmds(tctx);
    if (evicting_stack_get_size(undone) == 0) {
        return;
    }

    // TODO: extract it all into separate function
    struct CommandResult *popped_result = evicting_stack_pop_back(undone);
    struct Command       *inverse_command = command_create_from_opposing_result(popped_result);
    command_result_destroy(popped_result);
    struct CommandResult *result = command_exec(tctx->ctx, inverse_command);
    command_destroy(inverse_command);
    struct CommandResult *evicted_result = evicting_stack_push_back(done, result);
    command_result_destroy(evicted_result);
}