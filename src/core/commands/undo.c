#include "core/commands/undo.h"

#include <malloc.h>
#include <stdbool.h>
#include <time.h>

#include "collections/evicting_stack.h"
#include "core/_context.h"
#include "core/commands/commands.h"

#define UNDO_MAX_COUNT 50

struct UndoFacade {
    struct EvictingStack *done;
    struct EvictingStack *undone;
    struct Context       *ctx;
};

struct UndoFacade *_undo_facade_create_empty(void) {
    struct UndoFacade *undo_facade = malloc(sizeof(struct UndoFacade));
    undo_facade->done = NULL;
    undo_facade->undone = NULL;
    undo_facade->ctx = NULL;
    return undo_facade;
}

struct UndoFacade *undo_facade_create(void) {
    struct UndoFacade *undo_facade = _undo_facade_create_empty();
    undo_facade->done = evicting_stack_create(UNDO_MAX_COUNT);
    undo_facade->undone = evicting_stack_create(UNDO_MAX_COUNT);
    return undo_facade;
}

void undo_facade_set_ctx(struct UndoFacade *undo_facade, struct Context *ctx) {
    undo_facade->ctx = ctx;
}

void undo_facade_destroy(struct UndoFacade *undo_facade) {
    evicting_stack_destroy(undo_facade->done, command_result_destroy);
    evicting_stack_destroy(undo_facade->undone, command_result_destroy);
    undo_facade->done = NULL;
    undo_facade->undone = NULL;
    free(undo_facade);
}

void undo_facade_add_done(struct UndoFacade *undo_facade, struct CommandResult *result) {
    struct CommandResult *evicted_result = evicting_stack_push_back(undo_facade->done, result);
    command_result_destroy(evicted_result);
}

struct CommandResult *_undo_facade_pop_done(struct UndoFacade *undo_facade) {
    return evicting_stack_pop_back(undo_facade->done);
}

void _undo_facade_add_undone(struct UndoFacade *undo_facade, struct CommandResult *result) {
    struct CommandResult *evicted_result = evicting_stack_push_back(undo_facade->undone, result);
    command_result_destroy(evicted_result);
}

struct CommandResult *_undo_facade_pop_undone(struct UndoFacade *undo_facade) {
    return evicting_stack_pop_back(undo_facade->undone);
}

bool undo_facade_undo(struct UndoFacade *undo_facade) {
    if (undo_facade_is_done_empty(undo_facade)) {
        return false;
    }

    struct CommandResult *popped_result = _undo_facade_pop_done(undo_facade);
    struct Command       *inverse_command = command_create_from_opposing_result(popped_result);
    command_result_destroy(popped_result);
    struct CommandResult *result = command_exec(undo_facade->ctx, inverse_command);
    command_destroy(inverse_command);

    _undo_facade_add_undone(undo_facade, result);

    return true;
}

bool undo_facade_redo(struct UndoFacade *undo_facade) {
    if (undo_facade_is_undone_empty(undo_facade)) {
        return false;
    }

    struct CommandResult *popped_result = _undo_facade_pop_undone(undo_facade);
    struct Command       *inverse_command = command_create_from_opposing_result(popped_result);
    command_result_destroy(popped_result);
    struct CommandResult *result = command_exec(undo_facade->ctx, inverse_command);
    command_destroy(inverse_command);

    undo_facade_add_done(undo_facade, result);

    return true;
}

bool undo_facade_is_done_empty(const struct UndoFacade *undo_facade) {
    return evicting_stack_get_size(undo_facade->done) == 0;
}

bool undo_facade_is_undone_empty(const struct UndoFacade *undo_facade) {
    return evicting_stack_get_size(undo_facade->undone) == 0;
}
