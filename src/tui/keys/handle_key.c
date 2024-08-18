#include "tui/keys/handle_key.h"

#include <errno.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>

#include "collections/gap_buffer.h"
#include "core/commands.h"
#include "io.h"
#include "tui/context.h"
#include "tui/keys/handle_delete_key.h"
#include "tui/keys/handle_navigation_key.h"
#include "tui/keys/handle_other_key.h"
#include "tui/keys/key.h"
#include "tui/layout.h"
#include "tui/optionals.h"
#include "tui/text.h"

void handle_key(
    struct TuiContext *tctx,
    struct Board      *line_number_board,
    struct Board      *status_board,
    struct Board      *text_board,
    int                key
) {
    // TODO: handle in separate functions
    switch (key) {
        case KEY_RESIZE: {
            recreate_boards(tctx->ctx, line_number_board, status_board, text_board);
            break;
        }
        case CTRL('s'): {
            const char *filepath = tui_context_get_filepath(tctx);
            FILE       *file = fopen(filepath, "w+");
            gap_buffer_write_to_file(tui_context_get_gap_buffer(tctx), file);
            fclose(file);
            break;
        }
        case CTRL('z'): {
            struct EvictingStack *done = tui_context_get_done_cmds(tctx);
            struct EvictingStack *undone = tui_context_get_undone_cmds(tctx);
            if (evicting_stack_get_size(done) == 0) {
                break;
            }

            // TODO: extract it all into separate function
            struct CommandResult *popped_result = evicting_stack_pop_back(done);
            struct Command       *inverse_command = command_create_from_opposing_result(popped_result);
            command_result_destroy(popped_result);
            struct CommandResult *result = command_exec(tctx->ctx, inverse_command);
            command_destroy(inverse_command);
            struct CommandResult *evicted_result = evicting_stack_push_back(undone, result);
            command_result_destroy(evicted_result);
            break;
        }
        case CTRL('y'): {
            struct EvictingStack *undone = tui_context_get_undone_cmds(tctx);
            struct EvictingStack *done = tui_context_get_done_cmds(tctx);
            if (evicting_stack_get_size(undone) == 0) {
                break;
            }

            // TODO: extract it all into separate function
            struct CommandResult *popped_result = evicting_stack_pop_back(undone);
            struct Command       *inverse_command = command_create_from_opposing_result(popped_result);
            command_result_destroy(popped_result);
            struct CommandResult *result = command_exec(tctx->ctx, inverse_command);
            command_destroy(inverse_command);
            struct CommandResult *evicted_result = evicting_stack_push_back(done, result);
            command_result_destroy(evicted_result);
            break;
        }
        case KEY_DOWN:
        case KEY_UP:
        case KEY_LEFT:
        case KEY_RIGHT: {
            enum NavigationRequest request = handle_navigation_key(tctx, text_board, key);
            if (request != NAVREQ_NO) {
                fulfill_navigation_request(tctx, request);
            }
            break;
        }
        case KEY_DC:
        case KEY_BACKSPACE: {
            handle_delete_key(tctx, text_board, key);
            break;
        }
        default: {
            handle_other(tctx, text_board, key);
            break;
        }
    }
}
