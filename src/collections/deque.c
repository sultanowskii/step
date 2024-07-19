#include "collections/deque.h"

#include <malloc.h>
#include <stddef.h>

struct DequeNode {
    void             *value;
    struct DequeNode *prev;
    struct DequeNode *next;
};

struct DequeNode *deque_node_create_empty() {
    struct DequeNode *dnode = malloc(sizeof(struct DequeNode));
    dnode->value = NULL;
    dnode->next = NULL;
    dnode->prev = NULL;
    return dnode;
}

struct DequeNode *deque_node_create(void *value) {
    struct DequeNode *dnode = deque_node_create_empty();
    dnode->value = value;
    return dnode;
}

void deque_node_destroy(struct DequeNode *dnode) {
    free(dnode);
}

struct Deque {
    size_t            size;
    struct DequeNode *head;
    struct DequeNode *tail;
};

struct Deque *deque_create_emtpy() {
    struct Deque *deque = malloc(sizeof(struct Deque));
    deque->size = 0;
    deque->head = NULL;
    deque->tail = NULL;
    return deque;
}

struct Deque *deque_create() {
    struct Deque *deque = deque_create_emtpy();
    return deque;
}

void deque_destroy(struct Deque *deque, void (*destroy_value)(void *)) {
    while (deque_get_size(deque) != 0) {
        void *value = deque_pop_front(deque);
        destroy_value(value);
    }

    free(deque);
}

void _deque_size_inc(struct Deque *deque) {
    deque->size++;
}

void _deque_size_dec(struct Deque *deque) {
    deque->size--;
}

size_t deque_get_size(const struct Deque *deque) {
    return deque->size;
}

void deque_push_front(struct Deque *deque, void *value) {
    struct DequeNode *dnode = deque_node_create(value);

    if (deque_get_size(deque) == 0) {
        deque->head = dnode;
        deque->tail = dnode;
        _deque_size_inc(deque);
        return;
    }

    dnode->next = deque->head;
    dnode->next->prev = dnode;

    deque->head = dnode;

    _deque_size_inc(deque);
}

void *deque_pop_front(struct Deque *deque) {
    if (deque_get_size(deque) == 0) {
        return NULL;
    }

    struct DequeNode *dnode = deque->head;
    void             *value = dnode->value;

    if (deque_get_size(deque) == 1) {
        deque->head = NULL;
        deque->tail = NULL;
    } else {
        deque->head = dnode->next;
        deque->head->prev = NULL;
    }

    _deque_size_dec(deque);
    deque_node_destroy(dnode);
    return value;
}

void deque_push_back(struct Deque *deque, void *value) {
    struct DequeNode *dnode = deque_node_create(value);

    if (deque_get_size(deque) == 0) {
        deque->head = dnode;
        deque->tail = dnode;
        _deque_size_inc(deque);
        return;
    }

    dnode->prev = deque->tail;
    dnode->prev->next = dnode;

    deque->tail = dnode;

    _deque_size_inc(deque);
}

void *deque_pop_back(struct Deque *deque) {
    if (deque_get_size(deque) == 0) {
        return NULL;
    }

    struct DequeNode *dnode = deque->tail;
    void             *value = dnode->value;

    if (deque_get_size(deque) == 1) {
        deque->head = NULL;
        deque->tail = NULL;
    } else {
        deque->tail = dnode->prev;
        deque->tail->next = NULL;
    }

    _deque_size_dec(deque);
    deque_node_destroy(dnode);
    return value;
}

void deque_print(const struct Deque *deque, void (*print_value)(void *)) {
    printf("[ <-> ");

    struct DequeNode *dnode = deque->head;

    while (dnode != NULL) {
        print_value(dnode->value);
        printf(" <-> ");
        dnode = dnode->next;
    }

    printf("]\n");
}

void deque_debug_print(const struct Deque *deque, void (*print_value)(void *)) {
    printf("Deque (0x%p)\n", deque);
    printf(" size: %zu\n", deque_get_size(deque));
    printf(" head: %p\n", deque->head);
    printf(" tail: %p\n", deque->tail);
    printf(" nodes:\n");

    struct DequeNode *dnode = deque->head;
    size_t            cntr = 0;

    while (dnode != NULL) {
        printf("  DNode%zu (%p):\n", cntr, dnode);
        printf("   prev: %p\n", dnode->prev);
        printf("   next: %p\n", dnode->next);
        printf("   value: ");
        print_value(dnode->value);
        puts("");
        dnode = dnode->next;
        cntr++;
    }
}
