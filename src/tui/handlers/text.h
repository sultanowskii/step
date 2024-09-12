#pragma once

#include "tui/core/context.h"
#include "tui/events/event.h"

void handle_key_text(struct TuiContext *tctx, const struct EventKeyText *key_text);
