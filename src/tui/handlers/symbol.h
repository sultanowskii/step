#pragma once

#include "tui/core/_context.h"
#include "tui/events/event.h"

void handle_symbol_added(struct TuiContext *tctx, const struct EventSymbolAdded *symbol_added);
void handle_symbol_removed(struct TuiContext *tctx, const struct EventSymbolRemoved *symbol_removed);
