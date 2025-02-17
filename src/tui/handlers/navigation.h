#pragma once

#include "core/context.h"
#include "tui/events/event.h"

void handle_key_navigation(struct Context *ctx, const struct EventKeyNavigation *key_navigation);

void handle_request_go_up(struct Context *ctx, const struct EventRequestGoUp *request_go_up);
void handle_request_go_down(struct Context *ctx, const struct EventRequestGoDown *request_go_down);

void handle_request_go_to_bof(struct Context *ctx, const struct EventRequestGoToBof *request_go_to_bof);
void handle_request_go_to_eof(struct Context *ctx, const struct EventRequestGoToEof *request_go_to_eof);
