#include "collections/deque.h"

#include <malloc.h>
#include <stdbool.h>

#include "collections/evicting_deque.h"
#include "collections/print.h"
#include "nonstd/fmt.h"
#include "nonstd/mem.h"

struct Deque {
    struct EvictingDeque *edeque;
};

struct Deque *deque_create_empty() {
    struct Deque *deque = malloc(sizeof(struct Deque));
    deque->edeque = evicting_deque_create(0);
    return deque;
}

struct Deque *deque_create() {
    struct Deque *deque = deque_create_empty();
    return deque;
}

void deque_destroy(struct Deque *deque, destroy destroy_value) {
    evicting_deque_destroy(deque->edeque, destroy_value);
    deque->edeque = FREED_DUMMY;
    free(deque);
}

size_t deque_get_size(const struct Deque *deque) {
    return evicting_deque_get_size(deque->edeque);
}

size_t deque_get_max_size(const struct Deque *deque) {
    return evicting_deque_get_max_size(deque->edeque);
}

bool deque_is_size_limited(const struct Deque *deque) {
    return evicting_deque_is_size_limited(deque->edeque);
}

void deque_push_front(struct Deque *deque, void *value) {
    evicting_deque_push_front(deque->edeque, value);
}

void *deque_pop_front(struct Deque *deque) {
    return evicting_deque_pop_front(deque->edeque);
}

void deque_push_back(struct Deque *deque, void *value) {
    evicting_deque_push_back(deque->edeque, value);
}

void *deque_pop_back(struct Deque *deque) {
    return evicting_deque_pop_back(deque->edeque);
}

void deque_print(const struct Deque *deque, print print_value) {
    deque_print_with_indent(deque, print_value, 0);
}

void deque_print_with_indent(const struct Deque *deque, print print_value, size_t indent_size) {
    evicting_deque_print_with_indent(deque->edeque, print_value, indent_size);
}

void deque_debug_print(const struct Deque *deque, print print_value) {
    deque_debug_print_with_indent(deque, print_value, 0);
}

void deque_debug_print_with_indent(const struct Deque *deque, print print_value, size_t indent_size) {
    iprintf(indent_size, "Deque (0x%p)\n", deque);
    iprintf(indent_size, " edeque:\n");
    evicting_deque_debug_print_with_indent(deque->edeque, print_value, indent_size + 2);
}
