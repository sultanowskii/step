#pragma once

#include <ncurses.h>
#include <stddef.h>

#include "tui/boards/board.h"
#include "tui/coords.h"
#include "tui/core/context.h"

void update_status_board(struct TuiContext *tctx);
