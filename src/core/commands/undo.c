#include "core/commands/undo.h"

#include "collections/evicting_stack.h"
#include "core/commands/commands.h"
#include "core/context.h"

optional_command_result command_undo(struct Context *ctx) {
    struct EvictingStack *done = context_get_done_cmds(ctx);
    if (evicting_stack_get_size(done) == 0) {
        return command_result_none();
    }

    struct CommandResult *popped_result = evicting_stack_pop_back(done);
    struct Command       *inverse_command = command_create_from_opposing_result(popped_result);
    command_result_destroy(popped_result);
    struct CommandResult *result = command_exec(ctx, inverse_command);
    command_destroy(inverse_command);

    return command_result_some(result);
}

optional_command_result command_redo(struct Context *ctx) {
    struct EvictingStack *undone = context_get_undone_cmds(ctx);
    if (evicting_stack_get_size(undone) == 0) {
        return command_result_none();
    }

    struct CommandResult *popped_result = evicting_stack_pop_back(undone);
    struct Command       *inverse_command = command_create_from_opposing_result(popped_result);
    command_result_destroy(popped_result);
    struct CommandResult *result = command_exec(ctx, inverse_command);
    command_destroy(inverse_command);

    return command_result_some(result);
}
