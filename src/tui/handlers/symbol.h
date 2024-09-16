#pragma once

#include "core/_context.h"
#include "tui/events/event.h"

void handle_symbol_added(struct Context *ctx, const struct EventSymbolAdded *symbol_added);
void handle_symbol_removed(struct Context *ctx, const struct EventSymbolRemoved *symbol_removed);
