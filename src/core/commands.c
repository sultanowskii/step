#include "core/commands.h"

#include <stddef.h>

#include "core/context.h"
#include "core/state.h"
#include "runtime.h"

void _exec_insert(struct Context *ctx, struct CmdInsert *insert) {
    panic("insert: not implemented");
}

void _exec_delete(struct Context *ctx, struct CmdDelete *delete) {
    panic("delete: not implemented");
}

void _exec_save(struct Context *ctx, struct CmdSave *save) {
    panic("save: not implemented");
}

void _exec_exit(struct Context *ctx, struct CmdExit *delete) {
    context_set_state(ctx, STATE_EXIT);
}

void exec_command(struct Context *ctx, struct Command *command) {
    switch (command->type) {
    case CMD_INSERT: {
        _exec_insert(ctx, command->cmd.insert);
        break;
    };
    case CMD_DELETE: {
        _exec_delete(ctx, command->cmd.delete);
        break;
    };
    case CMD_SAVE: {
        _exec_save(ctx, command->cmd.save);
        break;
    };
    case CMD_EXIT: {
        _exec_exit(ctx, command->cmd.exit);
        break;
    };
    default: {
        panic("runtime error: unexpected command type");
    };
    }
}
