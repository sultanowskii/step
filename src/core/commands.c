#include "core/commands.h"

#include <malloc.h>
#include <stddef.h>

#include "collections/gap_buffer.h"
#include "core/context.h"
#include "core/state.h"
#include "runtime.h"
#include "str.h"

enum CommandType {
    CMD_INSERT,
    CMD_DELETE,
};

struct CmdInsert {
    size_t index;
    char  *s;
};

struct CmdInsertSymbol {
    size_t index;
    char   symbol;
};

struct CmdDelete {
    size_t index;
    size_t n;
};

struct Command {
    enum CommandType type;
    union {
        struct CmdInsert insert;
        struct CmdDelete delete;
    } cmd;
};

struct Command *command_create_empty() {
    struct Command *cmd = malloc(sizeof(struct Command));
    return cmd;
}

struct Command *command_create_insert(size_t index, const char *s) {
    struct Command *cmd = command_create_empty();
    cmd->type = CMD_INSERT;
    cmd->cmd.insert = (struct CmdInsert){
        .index = index,
        .s = str_dup(s),
    };
    return cmd;
}

struct Command *command_create_delete(size_t index, size_t n) {
    struct Command *cmd = command_create_empty();
    cmd->type = CMD_DELETE;
    cmd->cmd.delete = (struct CmdDelete){
        .index = index,
        .n = n,
    };
    return cmd;
}

void command_destroy(struct Command *cmd) {
    free(cmd);
}

void command_exec_insert(struct Context *ctx, struct CmdInsert *insert) {
    struct GapBuffer *gb = context_get_gap_buffer(ctx);
    gap_buffer_insert(gb, insert->index, insert->s);
}

void command_exec_delete(struct Context *ctx, struct CmdDelete *delete) {
    struct GapBuffer *gb = context_get_gap_buffer(ctx);
    gap_buffer_delete_n(gb, delete->index, delete->n);
}

void command_exec(struct Context *ctx, struct Command *command) {
    switch (command->type) {
        case CMD_INSERT: {
            command_exec_insert(ctx, &(command->cmd.insert));
            break;
        };
        case CMD_DELETE: {
            command_exec_delete(ctx, &(command->cmd.delete));
            break;
        };
        default: {
            panic("runtime error: unexpected command type");
        };
    }
}
