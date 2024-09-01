#pragma once

#include "nonstd/compile.h"
#include "nonstd/optional.h"
#include "tui/boards/board.h"
#include "tui/core/context.h"
#include "tui/optionals.h"

optional_char handle_other(
    struct TuiContext *tctx,
    int                key
);
