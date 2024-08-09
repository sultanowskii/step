#pragma once

#include <ncurses.h>
#include <stddef.h>

#include "collections/gap_buffer.h"
#include "tui/board.h"
#include "tui/coords.h"

enum NavigationRequest {
    NAVREQ_NO,
    NAVREQ_LOWER,
    NAVREQ_UPPER,
};

enum NavigationRequest handle_navigation_key(
    int            c,
    struct Coords *cursor,
    struct Board  *text_board
);

void fulfill_navigation_request(
    enum NavigationRequest  request,
    const struct GapBuffer *gb,
    size_t                 *starting_index,
    size_t                 *starting_line_index
);
