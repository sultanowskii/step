#pragma once

#include <stddef.h>

#include "core/context.h"

enum CommandType {
    CMD_INSERT,
    CMD_DELETE,
    CMD_SAVE,
    CMD_EXIT,
};

struct CmdInsert {
    char  *s;
    size_t index;
};

struct CmdInsertSymbol {
    char   symbol;
    size_t index;
};

struct CmdDelete {
    size_t n;
    size_t index;
};

struct CmdSave {
};

struct CmdExit {
};

// TODO: proper creation/destruction
struct Command {
    enum CommandType type;
    union {
        struct CmdInsert *insert;
        struct CmdDelete *delete;
        struct CmdSave *save;
        struct CmdExit *exit;
    } cmd;
};

void exec_command(struct Context *ctx, struct Command *command);
