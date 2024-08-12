#include "core/commands.h"

#include <malloc.h>
#include <stddef.h>

#include "collections/gap_buffer.h"
#include "core/context.h"
#include "core/state.h"
#include "runtime.h"
#include "str.h"

enum CommandType {
    CMD_INSERT_STRING,
    CMD_INSERT_SYMBOL,
    CMD_DELETE,
};

struct CmdInsertString {
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
        struct CmdInsertString insert_string;
        struct CmdInsertSymbol insert_symbol;
        struct CmdDelete delete;
    } cmd;
};

struct Command *command_create_empty() {
    struct Command *cmd = malloc(sizeof(struct Command));
    return cmd;
}

struct Command *command_create_insert_string(size_t index, const char *s) {
    struct Command *cmd = command_create_empty();
    cmd->type = CMD_INSERT_STRING;
    cmd->cmd.insert_string = (struct CmdInsertString){
        .index = index,
        .s = str_dup(s),
    };
    return cmd;
}

struct Command *command_create_insert_symbol(size_t index, char symbol) {
    struct Command *cmd = command_create_empty();
    cmd->type = CMD_INSERT_SYMBOL;
    cmd->cmd.insert_symbol = (struct CmdInsertSymbol){
        .index = index,
        .symbol = symbol,
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

void command_exec_insert_string(struct Context *ctx, struct CmdInsertString *insert_string) {
    struct GapBuffer *gb = context_get_gap_buffer(ctx);
    gap_buffer_insert(gb, insert_string->index, insert_string->s);
}

void command_exec_insert_symbol(struct Context *ctx, struct CmdInsertSymbol *insert_symbol) {
    struct GapBuffer *gb = context_get_gap_buffer(ctx);
    gap_buffer_insert_symbol(gb, insert_symbol->index, insert_symbol->symbol);
}

void command_exec_delete(struct Context *ctx, struct CmdDelete *delete) {
    struct GapBuffer *gb = context_get_gap_buffer(ctx);
    gap_buffer_delete_n(gb, delete->index, delete->n);
}

void command_exec(struct Context *ctx, struct Command *command) {
    switch (command->type) {
        case CMD_INSERT_STRING: {
            command_exec_insert_string(ctx, &(command->cmd.insert_string));
            break;
        };
        case CMD_INSERT_SYMBOL: {
            command_exec_insert_symbol(ctx, &(command->cmd.insert_symbol));
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
