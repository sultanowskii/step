#pragma once

#include <stddef.h>

#include "core/context.h"
#include "core/state.h"
#include "tui/coords.h"

// Context wrapper with TUI-related stuff.
// The struct is public for convenience
//
// Still, create/destroy shall be used
struct TuiContext {
    struct Context *ctx;
    // Index of the first symbol on the screen
    size_t buf_starting_symbol_index;
    // Index of the first line on the screen
    size_t buf_starting_line_index;
    // Current cursor position
    struct Coords *cursor;
};

struct TuiContext *tui_context_create(
    struct Context *ctx,
    size_t          buf_starting_symbol_index,
    size_t          buf_starting_line_index,
    struct Coords  *cursor
);
void tui_context_destroy(struct TuiContext *tctx);

struct EvictingStack *tui_context_get_done_cmds(const struct TuiContext *tctx);
struct EvictingStack *tui_context_get_undone_cmds(const struct TuiContext *tctx);
struct GapBuffer     *tui_context_get_gap_buffer(const struct TuiContext *tctx);
enum State            tui_context_get_state(const struct TuiContext *tctx);
void                  tui_context_set_state(struct TuiContext *tctx, enum State new_state);