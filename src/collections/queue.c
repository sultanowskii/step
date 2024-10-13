#include "collections/queue.h"

#include <malloc.h>
#include <stdbool.h>

#include "collections/print.h"
#include "nonstd/fmt.h"
#include "nonstd/mem.h"

struct QueueNode {
    void             *value;
    struct QueueNode *prev;
    struct QueueNode *next;
};

struct QueueNode *queue_node_create_empty(void) {
    struct QueueNode *node = malloc(sizeof(struct QueueNode));
    node->value = NULL;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

struct QueueNode *queue_node_create(void *value) {
    struct QueueNode *node = queue_node_create_empty();
    node->value = value;
    return node;
}

void queue_node_destroy(struct QueueNode *node) {
    node->value = NULL;
    node->prev = NULL;
    node->next = NULL;
    free(node);
}

// TODO: independent implementation?
struct Queue {
    size_t            size;
    struct QueueNode *head;
    struct QueueNode *tail;
};

struct Queue *queue_create_empty(void) {
    struct Queue *queue = malloc(sizeof(struct Queue));
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
    return queue;
}

struct Queue *queue_create(void) {
    struct Queue *queue = queue_create_empty();
    queue->size = 0;
    return queue;
}

void queue_destroy(struct Queue *queue, destroy destroy_value) {
    while (queue_get_size(queue) != 0) {
        void *value = queue_pop(queue);
        destroy_value(value);
    }

    queue->head = FREED_DUMMY;
    queue->tail = FREED_DUMMY;
    free(queue);
}

size_t queue_get_size(const struct Queue *queue) {
    return queue->size;
}

void _queue_size_inc(struct Queue *queue) {
    queue->size++;
}

void _queue_size_dec(struct Queue *queue) {
    queue->size--;
}

void queue_push(struct Queue *queue, void *value) {
    struct QueueNode *node = queue_node_create(value);

    if (queue_get_size(queue) == 0) {
        queue->head = node;
        queue->tail = node;
    } else {
        node->next = queue->head;
        node->next->prev = node;
        queue->head = node;
    }

    _queue_size_inc(queue);
}

void *queue_pop(struct Queue *queue) {
    if (queue_get_size(queue) == 0) {
        return NULL;
    }

    struct QueueNode *node = queue->tail;
    if (queue_get_size(queue) == 1) {
        queue->head = NULL;
        queue->tail = NULL;
    } else {
        queue->tail = queue->tail->prev;
        queue->tail->next = NULL;
    }
    _queue_size_dec(queue);

    void *value = node->value;
    queue_node_destroy(node);
    return value;
}

void *queue_peek(struct Queue *queue) {
    if (queue_get_size(queue) == 0) {
        return NULL;
    }

    return queue->tail->value;
}

void queue_print(const struct Queue *queue, print print_value) {
    queue_print_with_indent(queue, print_value, 0);
}

void queue_print_with_indent(const struct Queue *queue, print print_value, size_t indent_size) {
    iprintf(indent_size, "[ -> ");

    struct QueueNode *node = queue->head;

    while (node != NULL) {
        print_value(node->value);
        printf(" -> ");
        node = node->next;
    }

    printf("]\n");
}

void queue_debug_print(const struct Queue *queue, print print_value) {
    queue_debug_print_with_indent(queue, print_value, 0);
}

void queue_debug_print_with_indent(const struct Queue *queue, print print_value, size_t indent_size) {
    iprintf(indent_size, "Queue (0x%p)\n", queue);
    iprintf(indent_size, " deque:\n");
    iprintf(indent_size, " size: %zu\n", queue_get_size(queue));
    iprintf(indent_size, " head: %p\n", queue->head);
    iprintf(indent_size, " tail: %p\n", queue->tail);
    iprintf(indent_size, " nodes:\n");

    struct QueueNode *node = queue->head;
    size_t            cntr = 0;

    while (node != NULL) {
        iprintf(indent_size + 2, "QueueNode%zu (%p):\n", cntr, node);
        iprintf(indent_size + 2, " prev: %p\n", node->prev);
        iprintf(indent_size + 2, " next: %p\n", node->next);
        iprintf(indent_size + 2, " value: ");
        print_value(node->value);
        print_newline();
        node = node->next;
        cntr++;
    }
}
