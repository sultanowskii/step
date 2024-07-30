#include "core/commands.h"

#include <stddef.h>

#include "core/context.h"
#include "runtime.h"

void _exec_insert(struct Context *ctx, struct Insert *insert) {
    panic("insert: not implemented");
}

void _exec_delete(struct Context *ctx, struct Delete *delete) {
    panic("delete: not implemented");
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
    default: {
        panic("runtime error: unexpected command type");
    };
    }
}
