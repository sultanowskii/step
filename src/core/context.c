#include "core/context.h"

#include <malloc.h>

#include "collections/evicting_stack.h"
#include "collections/gap_buffer.h"
#include "core/commands/undo.h"
#include "core/state.h"
#include "nonstd/str.h"

struct Context {
    enum State         state;
    struct UndoFacade *undo_facade;
    struct GapBuffer  *gap_buffer;
    char              *filepath;
};

struct Context *_context_create_empty(void) {
    struct Context *ctx = malloc(sizeof(struct Context));
    ctx->state = STATE_START;
    ctx->gap_buffer = NULL;
    ctx->filepath = NULL;
    return ctx;
}

struct Context *context_create(
    enum State         state,
    struct UndoFacade *undo_facade,
    struct GapBuffer  *gap_buffer,
    const char        *filepath // ah yes, trailing comma is prohibited with seemingly no explanation
) {
    struct Context *ctx = _context_create_empty();
    ctx->undo_facade = undo_facade;
    ctx->state = state;
    ctx->gap_buffer = gap_buffer;
    ctx->filepath = str_dup(filepath);
    return ctx;
}

void context_destroy(struct Context *ctx) {
    free(ctx->filepath);
    ctx->undo_facade = NULL;
    ctx->filepath = NULL;
    ctx->gap_buffer = NULL;
    free(ctx);
}

struct UndoFacade *context_get_undo_facade(const struct Context *ctx) {
    return ctx->undo_facade;
}

struct GapBuffer *context_get_gap_buffer(const struct Context *ctx) {
    return ctx->gap_buffer;
}

enum State context_get_state(const struct Context *ctx) {
    return ctx->state;
}

void context_set_state(struct Context *ctx, enum State new_state) {
    ctx->state = new_state;
}

const char *context_get_filepath(const struct Context *ctx) {
    return ctx->filepath;
}

void context_set_filepath(struct Context *ctx, const char *new_filepath) {
    free(ctx->filepath);
    ctx->filepath = str_dup(new_filepath);
}
