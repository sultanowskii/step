#pragma once

#include <stdbool.h>
#include <stddef.h>

enum CommandType {
    CMD_INSERT_SYMBOL,
    CMD_INSERT_STRING,
    CMD_DELETE_SYMBOL,
    CMD_DELETE_STRING,
};

#include "core/_context.h"

struct Command;
struct CommandResult;

struct Command *command_create_insert_symbol(size_t index, char symbol);
struct Command *command_create_delete_symbol(size_t index);
struct Command *command_create_insert_string(size_t index, const char *s, size_t length);
struct Command *command_create_delete_string(size_t index, size_t length);
struct Command *command_create_from_opposing_result(struct CommandResult *result);
void            command_destroy(struct Command *cmd);

// Returns an index of command result.
//
// Currently, all commands have index, so no unexpected surprises here.
size_t           command_result_get_index(const struct CommandResult *result);
enum CommandType command_result_get_type(const struct CommandResult *result);

void command_result_destroy(struct CommandResult *result);

struct CommandResult *command_exec(struct Context *ctx, struct Command *command);
