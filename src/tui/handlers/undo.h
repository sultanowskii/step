#pragma once

#include "tui/core/context.h"
#include "tui/events/event.h"

void handle_key_undo(struct TuiContext *tctx, const struct EventKeyUndo *key_undo);
void handle_key_redo(struct TuiContext *tctx, const struct EventKeyRedo *key_redo);
