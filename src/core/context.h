#pragma once

#include "core/_context.h"

#include "collections/evicting_stack.h"
#include "collections/gap_buffer.h"
#include "core/commands/undo.h"
#include "core/state.h"

// Creates context with specified done, undone stack and gap_buffer gap buffer.
struct Context *context_create(
    enum State         state,
    struct UndoFacade *undo_facade,
    struct GapBuffer  *gap_buffer,
    const char        *filepath
);

// Destroys context. Some fields must be destroyed manually beforehand:
// - undo_facade
// - gap_buffer
void context_destroy(struct Context *ctx);

struct UndoFacade *context_get_undo_facade(const struct Context *ctx);
struct GapBuffer  *context_get_gap_buffer(const struct Context *ctx);
enum State         context_get_state(const struct Context *ctx);
void               context_set_state(struct Context *ctx, enum State new_state);
const char        *context_get_filepath(const struct Context *ctx);
void               context_set_filepath(struct Context *ctx, const char *new_filepath);
