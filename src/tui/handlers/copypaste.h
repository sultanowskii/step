#pragma once

#include "core/context.h"
#include "tui/events/event.h"

void handle_key_copy(struct Context *ctx, const struct EventKeyCopy *key_copy);
void handle_key_cut(struct Context *ctx, const struct EventKeyCut *key_cut);
void handle_key_paste(struct Context *ctx, const struct EventKeyPaste *key_paste);
void handle_request_delete_string(struct Context *ctx, const struct EventRequestDeleteString *request_delete_string);
