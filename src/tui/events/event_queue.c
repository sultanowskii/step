#include "tui/events/event_queue.h"

#include <malloc.h>
#include <stdbool.h>

#include "collections/queue.h"
#include "nonstd/mem.h"
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
    queue_destroy(event_queue->queue, (func_destroy)event_destroy);
    event_queue->queue = NULL;
    free(event_queue);
}

void event_queue_push_symbol_added(struct EventQueue *event_queue, size_t index, char symbol) {
    struct Event *event = event_create_symbol_added(index, symbol);
    queue_push(event_queue->queue, event);
}

void event_queue_push_symbol_removed(struct EventQueue *event_queue, size_t index, char symbol) {
    struct Event *event = event_create_symbol_removed(index, symbol);
    queue_push(event_queue->queue, event);
}

void event_queue_push_key_undo(struct EventQueue *event_queue) {
    struct Event *event = event_create_key_undo();
    queue_push(event_queue->queue, event);
}

void event_queue_push_key_redo(struct EventQueue *event_queue) {
    struct Event *event = event_create_key_redo();
    queue_push(event_queue->queue, event);
}

void event_queue_push_key_delete(struct EventQueue *event_queue) {
    struct Event *event = event_create_key_delete();
    queue_push(event_queue->queue, event);
}

void event_queue_push_key_backspace(struct EventQueue *event_queue) {
    struct Event *event = event_create_key_backspace();
    queue_push(event_queue->queue, event);
}

void event_queue_push_key_text(struct EventQueue *event_queue, int key) {
    struct Event *event = event_create_key_text(key);
    queue_push(event_queue->queue, event);
}

void event_queue_push_key_navigation(struct EventQueue *event_queue, int key) {
    struct Event *event = event_create_key_navigation(key);
    queue_push(event_queue->queue, event);
}

void event_queue_push_request_go_up(struct EventQueue *event_queue) {
    struct Event *event = event_create_request_go_up();
    queue_push(event_queue->queue, event);
}

void event_queue_push_request_go_down(struct EventQueue *event_queue) {
    struct Event *event = event_create_request_go_down();
    queue_push(event_queue->queue, event);
}

void event_queue_push_request_go_to_bof(struct EventQueue *event_queue) {
    struct Event *event = event_create_request_go_to_bof();
    queue_push(event_queue->queue, event);
}

void event_queue_push_request_go_to_eof(struct EventQueue *event_queue) {
    struct Event *event = event_create_request_go_to_eof();
    queue_push(event_queue->queue, event);
}

struct Event *event_queue_pop(struct EventQueue *event_queue) {
    return queue_pop(event_queue->queue);
}

bool event_queue_is_empty(const struct EventQueue *event_queue) {
    return queue_get_size(event_queue->queue) == 0;
}
