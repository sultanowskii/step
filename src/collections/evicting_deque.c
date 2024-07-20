#include "collections/evicting_deque.h"

#include <malloc.h>
#include <stdbool.h>
#include <stddef.h>

#include "collections/print.h"
#include "fmt.h"
#include "mem.h"
#include "runtime.h"

struct DequeNode;

void              _evicting_deque_push_front(struct EvictingDeque *edeque, struct DequeNode *dnode);
struct DequeNode *_evicting_deque_pop_front(struct EvictingDeque *edeque);
void              _evicting_deque_push_back(struct EvictingDeque *edeque, struct DequeNode *dnode);
struct DequeNode *_evicting_deque_pop_back(struct EvictingDeque *edeque);

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
    dnode->value = FREED_DUMMY;
    dnode->next = FREED_DUMMY;
    dnode->prev = FREED_DUMMY;
    free(dnode);
}

struct EvictingDeque {
    size_t            size;
    size_t            max_size;
    struct DequeNode *head;
    struct DequeNode *tail;
};

struct EvictingDeque *evicting_deque_create_emtpy() {
    struct EvictingDeque *edeque = malloc(sizeof(struct EvictingDeque));
    edeque->max_size = 0;
    edeque->size = 0;
    edeque->head = NULL;
    edeque->tail = NULL;
    return edeque;
}

struct EvictingDeque *evicting_deque_create(size_t max_size) {
    struct EvictingDeque *edeque = evicting_deque_create_emtpy();
    edeque->max_size = max_size;
    return edeque;
}

void evicting_deque_destroy(struct EvictingDeque *edeque, destroy destroy_value) {
    while (evicting_deque_get_size(edeque) != 0) {
        void *value = evicting_deque_pop_front(edeque);
        destroy_value(value);
    }

    edeque->size = 0;
    edeque->head = FREED_DUMMY;
    edeque->tail = FREED_DUMMY;
    free(edeque);
}

void _evicting_deque_size_inc(struct EvictingDeque *edeque) {
    edeque->size++;
}

void _evicting_deque_size_dec(struct EvictingDeque *edeque) {
    edeque->size--;
}

size_t evicting_deque_get_size(const struct EvictingDeque *edeque) {
    return edeque->size;
}

size_t evicting_deque_get_max_size(const struct EvictingDeque *edeque) {
    return edeque->max_size;
}

size_t evicting_deque_is_size_limited(const struct EvictingDeque *edeque) {
    return evicting_deque_get_max_size(edeque) > 0;
}

void _evicting_deque_push_front(struct EvictingDeque *edeque, struct DequeNode *dnode) {
    if (evicting_deque_get_size(edeque) == 0) {
        edeque->head = dnode;
        edeque->tail = dnode;
    } else {
        dnode->next = edeque->head;
        dnode->next->prev = dnode;
        edeque->head = dnode;
    }

    _evicting_deque_size_inc(edeque);
}

void *evicting_deque_push_front(struct EvictingDeque *edeque, void *value) {
    struct DequeNode *dnode;
    void             *evicted_value = NULL;

    if (evicting_deque_is_size_limited(edeque) && evicting_deque_get_size(edeque) >= evicting_deque_get_max_size(edeque)) {
        dnode = _evicting_deque_pop_back(edeque);
        dnode->prev = NULL;
        dnode->next = NULL;
        evicted_value = dnode->value;
        dnode->value = value;
    } else {
        dnode = deque_node_create(value);
    }

    _evicting_deque_push_front(edeque, dnode);

    return evicted_value;
}

struct DequeNode *_evicting_deque_pop_front(struct EvictingDeque *edeque) {
    if (evicting_deque_get_size(edeque) == 0) {
        return NULL;
    }

    struct DequeNode *dnode = edeque->head;
    if (evicting_deque_get_size(edeque) == 1) {
        edeque->head = NULL;
        edeque->tail = NULL;
    } else {
        edeque->head = dnode->next;
        edeque->head->prev = NULL;
    }

    _evicting_deque_size_dec(edeque);
    return dnode;
}

void *evicting_deque_pop_front(struct EvictingDeque *edeque) {
    if (evicting_deque_get_size(edeque) == 0) {
        return NULL;
    }

    struct DequeNode *dnode = _evicting_deque_pop_front(edeque);
    if (dnode == NULL) {
        panic("edeque violated on pop_front()");
    }
    void *value = dnode->value;
    deque_node_destroy(dnode);

    return value;
}

void _evicting_deque_push_back(struct EvictingDeque *edeque, struct DequeNode *dnode) {
    if (evicting_deque_get_size(edeque) == 0) {
        edeque->head = dnode;
        edeque->tail = dnode;
    } else {
        dnode->prev = edeque->tail;
        dnode->prev->next = dnode;
        edeque->tail = dnode;
    }

    _evicting_deque_size_inc(edeque);
}

void *evicting_deque_push_back(struct EvictingDeque *edeque, void *value) {
    struct DequeNode *dnode;
    void             *evicted_value = NULL;

    if (evicting_deque_is_size_limited(edeque) && evicting_deque_get_size(edeque) >= evicting_deque_get_max_size(edeque)) {
        dnode = _evicting_deque_pop_front(edeque);
        dnode->prev = NULL;
        dnode->next = NULL;
        evicted_value = dnode->value;
        dnode->value = value;
    } else {
        dnode = deque_node_create(value);
    }

    _evicting_deque_push_back(edeque, dnode);

    return evicted_value;
}

struct DequeNode *_evicting_deque_pop_back(struct EvictingDeque *edeque) {
    if (evicting_deque_get_size(edeque) == 0) {
        return NULL;
    }

    struct DequeNode *dnode = edeque->tail;
    if (evicting_deque_get_size(edeque) == 1) {
        edeque->head = NULL;
        edeque->tail = NULL;
    } else {
        edeque->tail = dnode->prev;
        edeque->tail->next = NULL;
    }

    _evicting_deque_size_dec(edeque);
    return dnode;
}

void *evicting_deque_pop_back(struct EvictingDeque *edeque) {
    if (evicting_deque_get_size(edeque) == 0) {
        return NULL;
    }

    struct DequeNode *dnode = _evicting_deque_pop_back(edeque);
    if (dnode == NULL) {
        panic("edeque violated on pop_back()");
    }
    void *value = dnode->value;
    deque_node_destroy(dnode);

    return value;
}

void evicting_deque_print(const struct EvictingDeque *edeque, void (*print_value)(void *)) {
    evicting_deque_print_with_indent(edeque, print_value, 0);
}

void evicting_deque_print_with_indent(const struct EvictingDeque *edeque, print print_value, size_t indent_size) {
    iprintf(indent_size, "[ <-> ");

    struct DequeNode *dnode = edeque->head;

    while (dnode != NULL) {
        print_value(dnode->value);
        printf(" <-> ");
        dnode = dnode->next;
    }

    printf("]\n");
}

void evicting_deque_debug_print(const struct EvictingDeque *edeque, print print_value) {
    evicting_deque_debug_print_with_indent(edeque, print_value, 0);
}

void evicting_deque_debug_print_with_indent(const struct EvictingDeque *edeque, print print_value, size_t indent_size) {
    iprintf(indent_size, "EDeque (0x%p)\n", edeque);
    iprintf(indent_size, " size: %zu\n", evicting_deque_get_size(edeque));
    iprintf(indent_size, " head: %p\n", edeque->head);
    iprintf(indent_size, " tail: %p\n", edeque->tail);
    iprintf(indent_size, " nodes:\n");

    struct DequeNode *dnode = edeque->head;
    size_t            cntr = 0;

    while (dnode != NULL) {
        iprintf(indent_size + 2, "DNode%zu (%p):\n", cntr, dnode);
        iprintf(indent_size + 2, " prev: %p\n", dnode->prev);
        iprintf(indent_size + 2, " next: %p\n", dnode->next);
        iprintf(indent_size + 2, " value: ");
        print_value(dnode->value);
        print_newline();
        dnode = dnode->next;
        cntr++;
    }
}
