#pragma once

#include <stdbool.h>

#include "core/commands/commands.h"

struct UndoFacade;

struct UndoFacade *undo_facade_create(void);
void               undo_facade_set_ctx(struct UndoFacade *undo_facade, struct Context *ctx);
void               undo_facade_destroy(struct UndoFacade *undo_facade);

void undo_facade_add_done(struct UndoFacade *undo_facade, struct CommandResult *result);
bool undo_facade_undo(struct UndoFacade *undo_facade);
bool undo_facade_redo(struct UndoFacade *undo_facade);

bool undo_facade_is_done_empty(const struct UndoFacade *undo_facade);
bool undo_facade_is_undone_empty(const struct UndoFacade *undo_facade);
