#pragma once

#include "tui/core/_context.h"
#include "tui/events/event.h"

void handle_newline_added(struct TuiContext *tctx, struct EventNewlineAdded *newline_added);
void handle_newline_removed(struct TuiContext *tctx, struct EventNewlineRemoved *newline_removed);
