#include "core/commands.h"

#include <stddef.h>

#include "runtime.h"

void _exec_insert(struct Insert *insert) {
    panic("insert: not implemented");
}

void _exec_delete(struct Delete *delete) {
    panic("delete: not implemented");
}

void exec_command(struct Command *command) {
    switch (command->type) {
    case INSERT: {
        _exec_insert(command->cmd.insert);
        break;
    };
    case DELETE: {
        _exec_delete(command->cmd.delete);
        break;
    };
    default: {
        panic("runtime error: unexpected command type");
    };
    }
}
