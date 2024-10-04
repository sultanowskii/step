#pragma once

#include <stddef.h>

#include "core/_context.h"

struct Command;
struct CommandResult;

struct Command *command_create_insert_symbol(size_t index, char symbol);
struct Command *command_create_delete_symbol(size_t index);
struct Command *command_create_from_opposing_result(struct CommandResult *result);
void            command_destroy(struct Command *cmd);

// Returns an index of command result.
//
// Currently, all commands have index, so no unexpected surprises here.
size_t command_result_get_index(const struct CommandResult *result);

void command_result_destroy(struct CommandResult *result);

struct CommandResult *command_exec(struct Context *ctx, struct Command *command);
