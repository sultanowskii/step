#pragma once

#include "tui/core/context.h"
#include "tui/events/event.h"

void handle_key_delete(struct TuiContext *tctx, const struct EventKeyDelete *key_delete);
void handle_key_backspace(struct TuiContext *tctx, const struct EventKeyBackspace *key_backspace);
