#pragma once

#include <ncurses.h>
#include <stddef.h>

#include "collections/gap_buffer.h"
#include "tui/board.h"
#include "tui/context.h"
#include "tui/coords.h"

enum NavigationRequest {
    NAVREQ_NO,
    NAVREQ_LOWER,
    NAVREQ_UPPER,
};

enum NavigationRequest handle_navigation_key(
    struct TuiContext *tctx,
    struct Board      *text_board,
    int                c
);

void fulfill_navigation_request(
    struct TuiContext     *tctx,
    enum NavigationRequest request
);
