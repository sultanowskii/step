#pragma once

#include <stddef.h>

#include "core/context.h"
#include "tui/coords.h"

struct TuiContext {
    struct Context *ctx;
    // Index of the first symbol on the screen
    size_t buf_starting_symbol_index;
    // Index of the first line on the screen
    size_t buf_starting_line_index;
    // Current cursor position
    struct Coords *cursor;
};
