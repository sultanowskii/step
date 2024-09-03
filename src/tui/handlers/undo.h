#pragma once

#include "tui/core/context.h"
#include "tui/events/event.h"

void handle_key_undo(struct TuiContext *tctx, struct EventKeyUndo *key_undo);
void handle_key_redo(struct TuiContext *tctx, struct EventKeyRedo *key_redo);
