#pragma once

#include "tui/core/context.h"
#include "tui/events/event.h"
#include "tui/navigation.h"

void handle_key_navigation(struct TuiContext *tctx, const struct EventKeyNavigation *key_navigation);

void handle_request_go_up(struct TuiContext *tctx, const struct EventGoUpRequest *go_up_request);
void handle_request_go_down(struct TuiContext *tctx, const struct EventGoDownRequest *go_down_request);
