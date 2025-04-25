#include "core/commands/commands.h"

#include <malloc.h>
#include <stddef.h>
#include <string.h>
#include <time.h>

#include "collections/gap_buffer.h"
#include "core/context.h"
#include "nonstd/runtime.h"
#include "nonstd/str.h"

struct CmdInsertSymbol {
    size_t index;
    char   symbol;
};

struct CmdResultInsertSymbol {
    size_t index;
    char   symbol;
};

struct CmdInsertString {
    size_t index;
    char  *s;
    size_t length;
};

struct CmdResultInsertString {
    size_t index;
    char  *s;
    size_t length;
};

struct CmdDeleteSymbol {
    size_t index;
};

struct CmdResultDeleteSymbol {
    size_t index;
    char   symbol;
};

struct CmdDeleteString {
    size_t index;
    size_t length;
};

struct CmdResultDeleteString {
    size_t index;
    char  *s;
    size_t length;
};

struct Command {
    enum CommandType type;
    union {
        struct CmdInsertSymbol insert_symbol;
        struct CmdInsertString insert_string;
        struct CmdDeleteSymbol delete_symbol;
        struct CmdDeleteString delete_string;
    } body;
};

struct CommandResult {
    enum CommandType type;
    time_t           time;
    union {
        struct CmdResultInsertSymbol insert_symbol;
        struct CmdResultDeleteSymbol delete_symbol;
        struct CmdResultInsertString insert_string;
        struct CmdResultDeleteString delete_string;
    } body;
};

struct Command *command_create_empty(void) {
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

struct Command *command_create_insert_string(size_t index, const char *s, size_t length) {
    struct Command *cmd = command_create_empty();
    cmd->type = CMD_INSERT_STRING;
    cmd->body.insert_string = (struct CmdInsertString){
        .index = index,
        .s = str_dup(s),
        .length = length,
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

struct Command *command_create_delete_string(size_t index, size_t length) {
    struct Command *cmd = command_create_empty();
    cmd->type = CMD_DELETE_STRING;
    cmd->body.delete_string = (struct CmdDeleteString){
        .index = index,
        .length = length,
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
        case CMD_INSERT_STRING: {
            size_t index = result->body.insert_string.index;
            size_t length = result->body.insert_string.length;
            cmd = command_create_delete_string(index, length);
            break;
        };
        case CMD_DELETE_STRING: {
            size_t index = result->body.delete_string.index;
            char  *s = result->body.delete_string.s;
            size_t length = result->body.delete_string.length;
            cmd = command_create_insert_string(index, s, length);
            break;
        };
        default: {
            panic("runtime error: unexpected command type while creating command from opposing result");
        };
    }
    return cmd;
}

void command_destroy(struct Command *cmd) {
    switch (cmd->type) {
        case CMD_INSERT_SYMBOL:
        case CMD_DELETE_SYMBOL: {
            break;
        }
        case CMD_INSERT_STRING: {
            free(cmd->body.insert_string.s);
        }
        case CMD_DELETE_STRING:
        default: {
            break;
        }
    }
    free(cmd);
}

struct CommandResult *command_result_create_empty(void) {
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

struct CommandResult *command_result_create_insert_string(size_t index, char *s, size_t length) {
    struct CommandResult *result = command_result_create_empty();
    result->type = CMD_INSERT_STRING;
    result->body.insert_string = (struct CmdResultInsertString){
        .index = index,
        .s = s,
        .length = length,
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

struct CommandResult *command_result_create_delete_string(size_t index, char *s, size_t length) {
    struct CommandResult *result = command_result_create_empty();
    result->type = CMD_DELETE_STRING;
    result->body.delete_string = (struct CmdResultDeleteString){
        .index = index,
        .s = s,
        .length = length,
    };
    return result;
}

void command_result_destroy(struct CommandResult *result) {
    switch (result->type) {
        case CMD_INSERT_SYMBOL:
        case CMD_DELETE_SYMBOL: {
            break;
        }
        case CMD_INSERT_STRING: {
            free(result->body.insert_string.s);
            break;
        }
        case CMD_DELETE_STRING: {
            free(result->body.delete_string.s);
            break;
        }
        default: {
            break;
        }
    }
    free(result);
}

struct CommandResult *command_exec_insert_symbol(struct Context *ctx, struct CmdInsertSymbol *insert_symbol) {
    struct GapBuffer *gb = ctx->gap_buffer;
    size_t            index = insert_symbol->index;
    char              symbol = insert_symbol->symbol;

    gap_buffer_insert_symbol(gb, index, symbol);
    if (symbol == '\n') {
        ctx->line_count++;
    }
    return command_result_create_insert_symbol(index, symbol);
}

struct CommandResult *command_exec_insert_string(struct Context *ctx, struct CmdInsertString *insert_string) {
    struct GapBuffer *gb = ctx->gap_buffer;
    size_t            index = insert_string->index;
    char             *s = insert_string->s;

    gap_buffer_insert(gb, index, s);
    // TODO: calculate starting_line_index
    ctx->line_count += str_count(s, '\n');
    return command_result_create_insert_string(index, str_dup(s), strlen(s));
}

struct CommandResult *command_exec_delete_symbol(struct Context *ctx, struct CmdDeleteSymbol *delete_symbol) {
    struct GapBuffer *gb = ctx->gap_buffer;
    size_t            index = delete_symbol->index;
    size_t            symbol = gap_buffer_get_at(gb, index);

    gap_buffer_delete(gb, index);
    if (symbol == '\n') {
        ctx->line_count--;
    }
    return command_result_create_delete_symbol(index, symbol);
}

struct CommandResult *command_exec_delete_string(struct Context *ctx, struct CmdDeleteString *delete_string) {
    struct GapBuffer *gb = ctx->gap_buffer;
    size_t            index = delete_string->index;
    size_t            length = delete_string->length;

    char *buffer = malloc(length + 1);
    gap_buffer_strncpy_from(gb, buffer, index, length);
    // TODO: calculate starting_line_index
    ctx->line_count -= str_count(buffer, '\n');
    gap_buffer_delete_n(gb, index, length);
    return command_result_create_delete_string(index, buffer, length);
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
        case CMD_INSERT_STRING: {
            result = command_exec_insert_string(ctx, &(command->body.insert_string));
            break;
        };
        case CMD_DELETE_STRING: {
            result = command_exec_delete_string(ctx, &(command->body.delete_string));
            break;
        };
        default: {
            panic("runtime error: unexpected command type while executing command");
        };
    }

    result->time = time(NULL);

    return result;
}

size_t command_result_get_index(const struct CommandResult *result) {
    switch (result->type) {
        case CMD_INSERT_SYMBOL:
            return result->body.insert_symbol.index;
        case CMD_DELETE_SYMBOL:
            return result->body.delete_symbol.index;
        case CMD_INSERT_STRING:
            return result->body.insert_string.index;
        case CMD_DELETE_STRING:
            return result->body.delete_string.index;
        default: {
            panic("runtime error: unexpected command type while executing command");
        };
    }
    return -1;
}

enum CommandType command_result_get_type(const struct CommandResult *result) {
    return result->type;
}
