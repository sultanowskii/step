#pragma once

#include <stddef.h>

#include "core/context.h"

struct Command;

struct Command *command_create_insert(size_t index, const char *s);
struct Command *command_create_delete(size_t index, size_t n);
void            command_destroy(struct Command *cmd);

void command_exec(struct Context *ctx, struct Command *command);
