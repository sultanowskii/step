#include "collections/evicting_stack.h"

#include <malloc.h>
#include <stdbool.h>

#include "collections/evicting_deque.h"
#include "collections/print.h"
#include "nonstd/fmt.h"
#include "nonstd/mem.h"

struct EvictingStack {
    struct EvictingDeque *edeque;
};

struct EvictingStack *evicting_stack_create(size_t max_size) {
    struct EvictingStack *estack = malloc(sizeof(struct EvictingStack));
    estack->edeque = evicting_deque_create(max_size);
    return estack;
}

void evicting_stack_destroy(struct EvictingStack *estack, destroy destroy_value) {
    evicting_deque_destroy(estack->edeque, destroy_value);
    estack->edeque = FREED_DUMMY;
    free(estack);
}

size_t evicting_stack_get_size(const struct EvictingStack *estack) {
    return evicting_deque_get_size(estack->edeque);
}

size_t evicting_stack_get_max_size(const struct EvictingStack *estack) {
    return evicting_deque_get_max_size(estack->edeque);
}

bool evicting_stack_is_size_limited(const struct EvictingStack *estack) {
    return evicting_deque_is_size_limited(estack->edeque);
}

void *evicting_stack_push_back(struct EvictingStack *estack, void *value) {
    return evicting_deque_push_back(estack->edeque, value);
}

void *evicting_stack_pop_back(struct EvictingStack *estack) {
    return evicting_deque_pop_back(estack->edeque);
}

void *evicting_stack_peek_back(struct EvictingStack *estack) {
    return evicting_deque_peek_back(estack->edeque);
}

void *evicting_stack_peek_back_offset(struct EvictingStack *estack, size_t offset) {
    return evicting_deque_peek_back_offset(estack->edeque, offset);
}

void evicting_stack_print(const struct EvictingStack *estack, print print_value) {
    evicting_stack_print_with_indent(estack, print_value, 0);
}

void evicting_stack_print_with_indent(const struct EvictingStack *estack, print print_value, size_t indent_size) {
    evicting_deque_print_with_indent(estack->edeque, print_value, indent_size);
}

void evicting_stack_debug_print(const struct EvictingStack *estack, print print_value) {
    evicting_stack_debug_print_with_indent(estack, print_value, 0);
}

void evicting_stack_debug_print_with_indent(const struct EvictingStack *estack, print print_value, size_t indent_size) {
    iprintf(indent_size, "EStack (0x%p)\n", estack);
    iprintf(indent_size, " edeque:\n");
    evicting_deque_debug_print_with_indent(estack->edeque, print_value, indent_size + 2);
}
