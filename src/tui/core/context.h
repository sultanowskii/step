#pragma once

#include <stddef.h>

#include "core/commands/undo.h"
#include "core/context.h"
#include "core/state.h"
#include "tui/boards/board.h"
#include "tui/coords.h"

// Context wrapper with TUI-related stuff.
// The struct is public for convenience
//
// Still, create/destroy shall be used
struct TuiContext {
    struct Context *ctx;
    // Line number board
    struct Board *line_number_board;
    // Status board
    struct Board *status_board;
    // Main text board
    struct Board *text_board;
    // Index of the first symbol on the screen
    size_t starting_symbol_index;
    // Index of the first line on the screen
    size_t starting_line_index;
    // Current cursor position
    struct Coords *cursor;
};

struct TuiContext *tui_context_create(
    struct Context *ctx,
    struct Board   *line_number_board,
    struct Board   *status_board,
    struct Board   *text_board,
    size_t          starting_symbol_index,
    size_t          starting_line_index,
    struct Coords  *cursor
);
void tui_context_destroy(struct TuiContext *tctx);

struct UndoFacade *tui_context_get_undo_facade(const struct TuiContext *tctx);
struct GapBuffer  *tui_context_get_gap_buffer(const struct TuiContext *tctx);
enum State         tui_context_get_state(const struct TuiContext *tctx);
void               tui_context_set_state(struct TuiContext *tctx, enum State new_state);
const char        *tui_context_get_filepath(const struct TuiContext *tctx);
void               tui_context_set_filepath(struct TuiContext *tctx, const char *new_filepath);
