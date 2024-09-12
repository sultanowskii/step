#include "tui/events/event_queue.h"

#include <malloc.h>
#include <stdbool.h>

#include "collections/queue.h"
#include "tui/events/event.h"

struct EventQueue {
    struct Queue *queue;
};

struct EventQueue *event_queue_create_empty(void) {
    struct EventQueue *event_queue = malloc(sizeof(struct EventQueue));
    event_queue->queue = NULL;
    return event_queue;
}

struct EventQueue *event_queue_create(void) {
    struct EventQueue *event_queue = event_queue_create_empty();
    event_queue->queue = queue_create();
    return event_queue;
}

void event_queue_destroy(struct EventQueue *event_queue) {
    queue_destroy(event_queue->queue, event_destroy);
    event_queue->queue = NULL;
    free(event_queue);
}

void event_queue_push_newline_added(struct EventQueue *event_queue) {
    struct Event *event = event_create_newline_added();
    queue_push_back(event_queue->queue, event);
}

void event_queue_push_newline_removed(struct EventQueue *event_queue) {
    struct Event *event = event_create_newline_removed();
    queue_push_back(event_queue->queue, event);
}

void event_queue_push_key_undo(struct EventQueue *event_queue) {
    struct Event *event = event_create_key_undo();
    queue_push_back(event_queue->queue, event);
}

void event_queue_push_key_redo(struct EventQueue *event_queue) {
    struct Event *event = event_create_key_redo();
    queue_push_back(event_queue->queue, event);
}

void event_queue_push_key_delete(struct EventQueue *event_queue) {
    struct Event *event = event_create_key_delete();
    queue_push_back(event_queue->queue, event);
}

void event_queue_push_key_backspace(struct EventQueue *event_queue) {
    struct Event *event = event_create_key_backspace();
    queue_push_back(event_queue->queue, event);
}

struct Event *event_queue_pop(struct EventQueue *event_queue) {
    return queue_pop_front(event_queue->queue);
}

bool event_queue_is_empty(const struct EventQueue *event_queue) {
    return queue_get_size(event_queue->queue) == 0;
}
