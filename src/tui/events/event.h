#pragma once

#include <stddef.h>

#include "tui/core/_context.h"

struct Event;

struct EventNewlineAdded {
    size_t prev_line_count;
};

struct EventNewlineRemoved {
    size_t prev_line_count;
};

struct EventKeyUndo {
};

struct EventKeyRedo {
};

struct EventHandler {
    void (*handle_newline_added)(struct TuiContext *tctx, struct EventNewlineAdded *newline_added);
    void (*handle_newline_removed)(struct TuiContext *tctx, struct EventNewlineRemoved *newline_removed);
    void (*handle_key_undo)(struct TuiContext *tctx, struct EventKeyUndo *key_undo);
    void (*handle_key_redo)(struct TuiContext *tctx, struct EventKeyRedo *key_redo);
};

struct Event *event_create_newline_added(size_t prev_line_count);
struct Event *event_create_newline_removed(size_t prev_line_count);
struct Event *event_create_key_undo();
struct Event *event_create_key_redo();
void          event_destroy(struct Event *event);

void event_handle(const struct EventHandler *event_handler, struct TuiContext *tctx, struct Event *event);
