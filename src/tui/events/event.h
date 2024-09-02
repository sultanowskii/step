#pragma once

#include <stddef.h>

#include "tui/core/_context.h"

struct Event;

struct Event *event_create_newline_added(size_t prev_line_count);
struct Event *event_create_newline_removed(size_t prev_line_count);
void          event_destroy(struct Event *event);

void event_handle(struct TuiContext *tctx, struct Event *event);
