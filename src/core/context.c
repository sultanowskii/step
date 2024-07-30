#include "core/context.h"

#include <malloc.h>

#include "collections/evicting_stack.h"
#include "collections/gap_buffer.h"

struct Context {
    struct EvictingStack *done;
    struct EvictingStack *undone;
    struct GapBuffer     *text;
};

struct Context *context_create_empty() {
    struct Context *ctx = malloc(sizeof(struct Context));
    ctx->done = NULL;
    ctx->undone = NULL;
    ctx->text = NULL;
    return ctx;
}

struct Context *context_create(
    struct EvictingStack *done,
    struct EvictingStack *undone,
    struct GapBuffer     *text // ah yes, trailing comma is prohibited with seemingly no explanation
) {
    struct Context *ctx = context_create_empty();
    ctx->done = done;
    ctx->undone = undone;
    ctx->text = text;
    return ctx;
}

void context_destroy(struct Context *ctx) {
    ctx->done = NULL;
    ctx->undone = NULL;
    ctx->text = NULL;
    free(ctx);
}

struct EvictingStack *context_get_done(const struct Context *ctx) {
    return ctx->done;
}

struct EvictingStack *context_get_undone(const struct Context *ctx) {
    return ctx->undone;
}

struct GapBuffer *context_get_text(const struct Context *ctx) {
    return ctx->text;
}
