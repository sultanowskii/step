#pragma once

#include "core/context.h"
#include "tui/events/event.h"

void handle_key_delete(struct Context *ctx, const struct EventKeyDelete *key_delete);
void handle_key_backspace(struct Context *ctx, const struct EventKeyBackspace *key_backspace);
