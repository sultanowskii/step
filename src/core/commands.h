#pragma once

#include <stddef.h>

#include "core/context.h"

enum CommandType {
    CMD_INSERT,
    CMD_DELETE,
};

struct Insert {
    char  *s;
    size_t index;
};

struct Delete {
    size_t n;
    size_t index;
};

struct Command {
    enum CommandType type;
    union {
        struct Insert *insert;
        struct Delete *delete;
    } cmd;
};

void exec_command(struct Context *ctx, struct Command *command);
