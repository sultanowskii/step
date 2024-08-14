#include "tui/context.h"

#include <malloc.h>
#include <stddef.h>

#include "core/context.h"
#include "str.h"
#include "tui/coords.h"

struct TuiContext *tui_context_create_empty() {
    struct TuiContext *tctx = malloc(sizeof(struct TuiContext));
    tctx->ctx = NULL;
    tctx->starting_symbol_index = 0;
    tctx->starting_line_index = 0;
    tctx->cursor = NULL;
    return tctx;
}

struct TuiContext *tui_context_create(
    struct Context *ctx,
    size_t          starting_symbol_index,
    size_t          starting_line_index,
    struct Coords  *cursor
) {
    struct TuiContext *tctx = tui_context_create_empty();
    tctx->ctx = ctx;
    tctx->starting_symbol_index = starting_symbol_index;
    tctx->starting_line_index = starting_line_index;
    tctx->cursor = cursor;
    return tctx;
}

void tui_context_destroy(struct TuiContext *tctx) {
    tctx->ctx = NULL;
    tctx->cursor = NULL;
    free(tctx);
}

struct EvictingStack *tui_context_get_done_cmds(const struct TuiContext *tctx) {
    return context_get_done_cmds(tctx->ctx);
}

struct EvictingStack *tui_context_get_undone_cmds(const struct TuiContext *tctx) {
    return context_get_undone_cmds(tctx->ctx);
}

struct GapBuffer *tui_context_get_gap_buffer(const struct TuiContext *tctx) {
    return context_get_gap_buffer(tctx->ctx);
}

enum State tui_context_get_state(const struct TuiContext *tctx) {
    return context_get_state(tctx->ctx);
}

void tui_context_set_state(struct TuiContext *tctx, enum State new_state) {
    return context_set_state(tctx->ctx, new_state);
}

const char *tui_context_get_filepath(const struct TuiContext *tctx) {
    return context_get_filepath(tctx->ctx);
}

void tui_context_set_filepath(struct TuiContext *tctx, const char *new_filepath) {
    context_set_filepath(tctx->ctx, new_filepath);
}
