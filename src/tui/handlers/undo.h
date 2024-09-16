#pragma once

#include "core/context.h"
#include "tui/events/event.h"

void handle_key_undo(struct Context *ctx, const struct EventKeyUndo *key_undo);
void handle_key_redo(struct Context *ctx, const struct EventKeyRedo *key_redo);
