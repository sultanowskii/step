#include "collections/queue.h"

#include <malloc.h>
#include <stdbool.h>

#include "collections/deque.h"
#include "collections/print.h"
#include "nonstd/fmt.h"
#include "nonstd/mem.h"

// TODO: independent implementation?
struct Queue {
    struct Deque *deque;
};

struct Queue *queue_create_empty() {
    struct Queue *queue = malloc(sizeof(struct Queue));
    queue->deque = deque_create(0);
    return queue;
}

struct Queue *queue_create() {
    struct Queue *queue = queue_create_empty();
    return queue;
}

void queue_destroy(struct Queue *queue, destroy destroy_value) {
    deque_destroy(queue->deque, destroy_value);
    queue->deque = FREED_DUMMY;
    free(queue);
}

size_t queue_get_size(const struct Queue *queue) {
    return deque_get_size(queue->deque);
}

size_t queue_get_max_size(const struct Queue *queue) {
    return deque_get_max_size(queue->deque);
}

bool queue_is_size_limited(const struct Queue *queue) {
    return deque_is_size_limited(queue->deque);
}

void queue_push_front(struct Queue *queue, void *value) {
    deque_push_front(queue->deque, value);
}

void *queue_pop_front(struct Queue *queue) {
    return deque_pop_front(queue->deque);
}

void *queue_peek_front(struct Queue *queue) {
    return deque_peek_front(queue->deque);
}

void queue_push_back(struct Queue *queue, void *value) {
    deque_push_back(queue->deque, value);
}

void *queue_pop_back(struct Queue *queue) {
    return deque_pop_back(queue->deque);
}

void *queue_peek_back(struct Queue *queue) {
    return deque_peek_back(queue->deque);
}

void queue_print(const struct Queue *queue, print print_value) {
    queue_print_with_indent(queue, print_value, 0);
}

void queue_print_with_indent(const struct Queue *queue, print print_value, size_t indent_size) {
    deque_print_with_indent(queue->deque, print_value, indent_size);
}

void queue_debug_print(const struct Queue *queue, print print_value) {
    queue_debug_print_with_indent(queue, print_value, 0);
}

void queue_debug_print_with_indent(const struct Queue *queue, print print_value, size_t indent_size) {
    iprintf(indent_size, "Queue (0x%p)\n", queue);
    iprintf(indent_size, " deque:\n");
    deque_debug_print_with_indent(queue->deque, print_value, indent_size + 2);
}
