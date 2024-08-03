#pragma once

#include "collections/evicting_stack.h"
#include "collections/gap_buffer.h"
#include "core/state.h"

// Core context.
// Is used as a storage of several other structs - it doesn't manage them at all.
struct Context;

// Creates context with specified done, undone stack and gap_buffer gap buffer.
struct Context *context_create(
    enum State            state,
    struct EvictingStack *done_cmds,
    struct EvictingStack *undone_cmds,
    struct GapBuffer     *gap_buffer // ah yes, trailing comma is prohibited with seemingly no explanation
);

// Destroys context. Only destroys the structure itself - all fields must be destroyed manually beforehand.
void context_destroy(struct Context *ctx);

struct EvictingStack *context_get_done_cmds(const struct Context *ctx);
struct EvictingStack *context_get_undone_cmds(const struct Context *ctx);
struct GapBuffer     *context_get_gap_buffer(const struct Context *ctx);
enum State            context_get_state(const struct Context *ctx);
void                  context_set_state(struct Context *ctx, enum State new_state);
