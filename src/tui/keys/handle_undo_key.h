#pragma once

#include <stdbool.h>

#include "tui/core/context.h"

bool handle_undo_key(struct TuiContext *tctx);
bool handle_redo_key(struct TuiContext *tctx);
