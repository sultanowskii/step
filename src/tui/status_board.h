#pragma once

#include <ncurses.h>
#include <stddef.h>

#include "tui/board.h"
#include "tui/context.h"
#include "tui/coords.h"

void update_status_board(struct TuiContext *tctx, struct Board *status_board);
