#pragma once

#include "core/context.h"
#include "tui/events/event.h"
#include "tui/navigation.h"

void handle_key_navigation(struct Context *ctx, const struct EventKeyNavigation *key_navigation);

void handle_request_go_up(struct Context *ctx, const struct EventRequestGoUp *request_go_up);
void handle_request_go_down(struct Context *ctx, const struct EventRequestGoDown *request_go_down);
