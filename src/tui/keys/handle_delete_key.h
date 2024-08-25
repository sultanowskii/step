#pragma once

#include "tui/boards/board.h"
#include "tui/core/context.h"

void handle_delete_key(struct TuiContext *tctx, struct Board *text_board, int key);
