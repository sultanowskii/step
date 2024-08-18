#include "core/commands.h"

#include <malloc.h>
#include <stddef.h>

#include "collections/gap_buffer.h"
#include "core/context.h"
#include "core/state.h"
#include "runtime.h"
#include "str.h"

enum CommandType {
    CMD_INSERT_SYMBOL,
    CMD_DELETE_SYMBOL,
};

struct CmdInsertSymbol {
    size_t index;
    char   symbol;
};

struct CmdResultInsertSymbol {
    size_t index;
    char   symbol;
};

struct CmdDeleteSymbol {
    size_t index;
};

struct CmdResultDeleteSymbol {
    size_t index;
    char   symbol;
};

struct Command {
    enum CommandType type;
    union {
        struct CmdInsertSymbol insert_symbol;
        struct CmdDeleteSymbol delete_symbol;
    } body;
};

struct CommandResult {
    enum CommandType type;
    union {
        struct CmdResultInsertSymbol insert_symbol;
        struct CmdResultDeleteSymbol delete_symbol;
    } body;
};

struct Command *command_create_empty() {
    struct Command *cmd = malloc(sizeof(struct Command));
    return cmd;
}

struct Command *command_create_insert_symbol(size_t index, char symbol) {
    struct Command *cmd = command_create_empty();
    cmd->type = CMD_INSERT_SYMBOL;
    cmd->body.insert_symbol = (struct CmdInsertSymbol){
        .index = index,
        .symbol = symbol,
    };
    return cmd;
}

struct Command *command_create_delete_symbol(size_t index) {
    struct Command *cmd = command_create_empty();
    cmd->type = CMD_DELETE_SYMBOL;
    cmd->body.delete_symbol = (struct CmdDeleteSymbol){
        .index = index,
    };
    return cmd;
}

struct Command *command_create_from_opposing_result(struct CommandResult *result) {
    struct Command *cmd;
    switch (result->type) {
        case CMD_INSERT_SYMBOL: {
            size_t index = result->body.insert_symbol.index;
            cmd = command_create_delete_symbol(index);
            break;
        };
        case CMD_DELETE_SYMBOL: {
            size_t index = result->body.delete_symbol.index;
            char   symbol = result->body.delete_symbol.symbol;
            cmd = command_create_insert_symbol(index, symbol);
            break;
        };
        default: {
            panic("runtime error: unexpected command type while creating command from opposing result");
        };
    }
    return cmd;
}

void command_destroy(struct Command *cmd) {
    free(cmd);
}

struct CommandResult *command_result_create_empty() {
    struct CommandResult *result = malloc(sizeof(struct CommandResult));
    return result;
}

struct CommandResult *command_result_create_insert_symbol(size_t index, char symbol) {
    struct CommandResult *result = command_result_create_empty();
    result->type = CMD_INSERT_SYMBOL;
    result->body.insert_symbol = (struct CmdResultInsertSymbol){
        .index = index,
        .symbol = symbol,
    };
    return result;
}

struct CommandResult *command_result_create_delete_symbol(size_t index, char symbol) {
    struct CommandResult *result = command_result_create_empty();
    result->type = CMD_DELETE_SYMBOL;
    result->body.delete_symbol = (struct CmdResultDeleteSymbol){
        .index = index,
        .symbol = symbol,
    };
    return result;
}

void command_result_destroy(struct CommandResult *result) {
    free(result);
}

struct CommandResult *command_exec_insert_symbol(struct Context *ctx, struct CmdInsertSymbol *insert_symbol) {
    struct GapBuffer *gb = context_get_gap_buffer(ctx);
    size_t            index = insert_symbol->index;
    char              symbol = insert_symbol->symbol;

    gap_buffer_insert_symbol(gb, index, symbol);
    return command_result_create_insert_symbol(index, symbol);
}

struct CommandResult *command_exec_delete_symbol(struct Context *ctx, struct CmdDeleteSymbol *delete_symbol) {
    struct GapBuffer *gb = context_get_gap_buffer(ctx);
    size_t            index = delete_symbol->index;
    size_t            symbol = gap_buffer_get_at(gb, index);

    gap_buffer_delete(gb, index);

    return command_result_create_delete_symbol(index, symbol);
}

struct CommandResult *command_exec(struct Context *ctx, struct Command *command) {
    struct CommandResult *result = NULL;
    switch (command->type) {
        case CMD_INSERT_SYMBOL: {
            result = command_exec_insert_symbol(ctx, &(command->body.insert_symbol));
            break;
        };
        case CMD_DELETE_SYMBOL: {
            result = command_exec_delete_symbol(ctx, &(command->body.delete_symbol));
            break;
        };
        default: {
            panic("runtime error: unexpected command type while executing command");
        };
    }
    return result;
}
