#include "core/context.h"

#include <malloc.h>

#include "collections/evicting_stack.h"
#include "collections/gap_buffer.h"
#include "core/state.h"

struct Context {
    enum State            state;
    struct EvictingStack *done_cmds;
    struct EvictingStack *undone_cmds;
    struct GapBuffer     *text;
};

struct Context *context_create_empty() {
    struct Context *ctx = malloc(sizeof(struct Context));
    ctx->state = NULL;
    ctx->done_cmds = NULL;
    ctx->undone_cmds = NULL;
    ctx->text = NULL;
    return ctx;
}

struct Context *context_create(
    enum State            state,
    struct EvictingStack *done_cmds,
    struct EvictingStack *undone_cmds,
    struct GapBuffer     *text // ah yes, trailing comma is prohibited with seemingly no explanation
) {
    struct Context *ctx = context_create_empty();
    ctx->state = state;
    ctx->done_cmds = done_cmds;
    ctx->undone_cmds = undone_cmds;
    ctx->text = text;
    return ctx;
}

void context_destroy(struct Context *ctx) {
    ctx->done_cmds = NULL;
    ctx->undone_cmds = NULL;
    ctx->text = NULL;
    free(ctx);
}

struct EvictingStack *context_get_done_cmds(const struct Context *ctx) {
    return ctx->done_cmds;
}

struct EvictingStack *context_get_undone_cmds(const struct Context *ctx) {
    return ctx->undone_cmds;
}

struct GapBuffer *context_get_text(const struct Context *ctx) {
    return ctx->text;
}

enum State context_get_state(const struct Context *ctx) {
    return ctx->state;
}

void context_set_state(struct Context *ctx, enum State new_state) {
    ctx->state = new_state;
}
