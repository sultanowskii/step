#pragma once

#include "tui/core/context.h"
#include "tui/events/event.h"
#include "tui/navigation.h"

void handle_key_navigation(struct TuiContext *tctx, const struct EventKeyNavigation *key_navigation);
void handle_singular_navigation_request(struct TuiContext *tctx, const struct EventSingularNavigationRequest *singular_navigation_request);