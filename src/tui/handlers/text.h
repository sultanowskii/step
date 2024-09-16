#pragma once

#include "core/context.h"
#include "tui/events/event.h"

void handle_key_text(struct Context *ctx, const struct EventKeyText *key_text);
