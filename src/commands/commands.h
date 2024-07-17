#pragma once

#include <stddef.h>

enum CommandType {
    INSERT,
    DELETE,
};

struct Insert {
    char  *s;
    size_t pos;
};

struct Delete {
    size_t pos;
    size_t n;
};

struct Command {
    enum CommandType type;
    union {
        struct Insert *insert;
        struct Delete *delete;
    } cmd;
};

void exec_command(struct Command *command);
