#include "core/commands/undo.h"

#include <malloc.h>
#include <stdbool.h>
#include <time.h>

#include "collections/evicting_stack.h"
#include "core/_context.h"
#include "core/commands/commands.h"
#include "nonstd/math.h"
#include "nonstd/optionals.h"

// Delimiter is used to divide done/undone stacks into a groups of operations.
#define DELIMITER ((void *)0xDDDDDDDD)

// Max number of undo commands to be stored
const size_t UNDO_COMMAND_MAX_COUNT = 10000;
// Max size of one undo group
const size_t UNDO_GROUP_MAX_SIZE = 10;

void command_result_or_delimiter_destroy(struct CommandResult *result) {
    if (result == DELIMITER) {
        return;
    }

    command_result_destroy(result);
}

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
    undo_facade->done = evicting_stack_create(UNDO_COMMAND_MAX_COUNT);
    undo_facade->undone = evicting_stack_create(UNDO_COMMAND_MAX_COUNT);
    return undo_facade;
}

void undo_facade_set_ctx(struct UndoFacade *undo_facade, struct Context *ctx) {
    undo_facade->ctx = ctx;
}

void undo_facade_destroy(struct UndoFacade *undo_facade) {
    evicting_stack_destroy(undo_facade->done, command_result_or_delimiter_destroy);
    evicting_stack_destroy(undo_facade->undone, command_result_or_delimiter_destroy);
    undo_facade->done = NULL;
    undo_facade->undone = NULL;
    free(undo_facade);
}

// Returns a number of consecutive done commands.
size_t count_consecutive_done_commands(const struct UndoFacade *undo_facade) {
    size_t cntr = 0;

    for (size_t offset = 0; offset < evicting_stack_get_size(undo_facade->done); offset++) {
        if (evicting_stack_peek_back_offset(undo_facade->done, offset) == DELIMITER) {
            return cntr;
        }
        cntr++;
    }

    return cntr;
}

// Tells whether it's required to insert DELIMITER before adding any more command results.
bool is_delimiter_required(struct UndoFacade *undo_facade, struct CommandResult *result) {
    if (evicting_stack_get_size(undo_facade->done) == 0) {
        return false;
    }

    if (count_consecutive_done_commands(undo_facade) > UNDO_GROUP_MAX_SIZE) {
        return true;
    }

    struct CommandResult *last_result = evicting_stack_peek_back(undo_facade->done);
    if (last_result == DELIMITER) {
        return false;
    }

    enum CommandType last_result_type = command_result_get_type(last_result);
    enum CommandType result_type = command_result_get_type(result);
    if (result_type != last_result_type) {
        return true;
    }

    size_t result_index = command_result_get_index(result);
    size_t last_result_index = command_result_get_index(last_result);

    switch (result_type) {
        case CMD_INSERT_SYMBOL: {
            if (last_result_index + 1 != result_index) {
                return true;
            }
            break;
        };
        case CMD_DELETE_SYMBOL: {
            if (result_index != last_result_index - 1) {
                return true;
            }
            break;
        };
    }

    return false;
}

void undo_facade_add_done(struct UndoFacade *undo_facade, struct CommandResult *result) {
    if (is_delimiter_required(undo_facade, result)) {
        evicting_stack_push_back(undo_facade->done, DELIMITER);
    }

    struct CommandResult *evicted_result = evicting_stack_push_back(undo_facade->done, result);
    command_result_or_delimiter_destroy(evicted_result);
}

void _undo_facade_add_undone(struct UndoFacade *undo_facade, struct CommandResult *result) {
    struct CommandResult *evicted_result = evicting_stack_push_back(undo_facade->undone, result);
    command_result_or_delimiter_destroy(evicted_result);
}

optional_size_t undo_facade_undo(struct UndoFacade *undo_facade) {
    if (undo_facade_is_done_empty(undo_facade)) {
        return size_t_none();
    }

    // We never expect DELIMITER to be on top of done
    if (evicting_stack_peek_back(undo_facade->done) == DELIMITER) {
        // TODO: unintended behavior
    }

    optional_size_t last_index = size_t_none();

    while (!undo_facade_is_done_empty(undo_facade)) {
        struct CommandResult *popped_result = evicting_stack_pop_back(undo_facade->done);
        if (popped_result == DELIMITER) {
            break;
        }

        struct Command *inverse_command = command_create_from_opposing_result(popped_result);
        command_result_or_delimiter_destroy(popped_result);
        struct CommandResult *result = command_exec(undo_facade->ctx, inverse_command);
        command_destroy(inverse_command);

        _undo_facade_add_undone(undo_facade, result);

        last_index = size_t_some(command_result_get_index(result));
    }

    _undo_facade_add_undone(undo_facade, DELIMITER);

    return last_index;
}

optional_size_t undo_facade_redo(struct UndoFacade *undo_facade) {
    if (undo_facade_is_undone_empty(undo_facade)) {
        return size_t_none();
    }

    // We always expect DELIMITER to be on top of undone
    if (evicting_stack_peek_back(undo_facade->undone) != DELIMITER) {
        // TODO: unintended behavior
    }

    // Moving delimiter from undone to done
    evicting_stack_push_back(undo_facade->done, evicting_stack_pop_back(undo_facade->undone));

    optional_size_t last_index = size_t_none();

    // Redo until we meet DELIMITER or bottom of undone
    while (!undo_facade_is_undone_empty(undo_facade) && evicting_stack_peek_back(undo_facade->undone) != DELIMITER) {
        struct CommandResult *popped_result = evicting_stack_pop_back(undo_facade->undone);
        struct Command       *inverse_command = command_create_from_opposing_result(popped_result);
        command_result_destroy(popped_result);
        struct CommandResult *result = command_exec(undo_facade->ctx, inverse_command);
        command_destroy(inverse_command);

        undo_facade_add_done(undo_facade, result);

        last_index = size_t_some(command_result_get_index(result));
    }

    return last_index;
}

bool undo_facade_is_done_empty(const struct UndoFacade *undo_facade) {
    return evicting_stack_get_size(undo_facade->done) == 0;
}

bool undo_facade_is_undone_empty(const struct UndoFacade *undo_facade) {
    return evicting_stack_get_size(undo_facade->undone) == 0;
}
