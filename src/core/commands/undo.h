#pragma once

#include "compile.h"
#include "core/commands/commands.h"
#include "core/context.h"
#include "optional.h"

IGNORE_UNUSED()
DECLARE_OPTIONAL(struct CommandResult *, command_result)

optional_command_result command_undo(struct Context *ctx);
optional_command_result command_redo(struct Context *ctx);
