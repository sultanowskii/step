#include "collections/evicting_stack.h"

#include <malloc.h>

#include "collections/deque.h"
#include "collections/print.h"
#include "fmt.h"
#include "mem.h"

struct EvictingStack {
    struct Deque *deque;
    size_t        max_size;
};

struct EvictingStack *evicting_stack_create_empty() {
    struct EvictingStack *estack = malloc(sizeof(struct EvictingStack));
    estack->max_size = 0;
    estack->deque = deque_create();
    return estack;
}

struct EvictingStack *evicting_stack_create(size_t max_size) {
    struct EvictingStack *estack = evicting_stack_create_empty();
    estack->max_size = max_size;
    return estack;
}

void evicting_stack_destroy(struct EvictingStack *estack, destroy destroy_value) {
    deque_destroy(estack->deque, destroy_value);
    estack->deque = FREED_DUMMY;
    estack->max_size = 0;
    free(estack);
}

size_t evicting_stack_get_size(const struct EvictingStack *estack) {
    return deque_get_size(estack->deque);
}

size_t evicting_stack_get_max_size(const struct EvictingStack *estack) {
    return estack->max_size;
}

void *evicting_stack_push_back(struct EvictingStack *estack, void *value) {
    deque_push_back(estack->deque, value);

    if (deque_get_size(estack->deque) <= estack->max_size) {
        return NULL;
    }

    return deque_pop_front(estack->deque);
}

void *evicting_stack_pop_back(struct EvictingStack *estack) {
    return deque_pop_back(estack->deque);
}

void evicting_stack_print(const struct EvictingStack *estack, print print_value) {
    evicting_stack_print_with_indent(estack, print_value, 0);
}

void evicting_stack_print_with_indent(const struct EvictingStack *estack, print print_value, size_t indent_size) {
    deque_print_with_indent(estack->deque, print_value, indent_size);
}

void evicting_stack_debug_print(const struct EvictingStack *estack, print print_value) {
    evicting_stack_debug_print_with_indent(estack, print_value, 0);
}

void evicting_stack_debug_print_with_indent(const struct EvictingStack *estack, print print_value, size_t indent_size) {
    iprintf(indent_size, "EStack (0x%p)\n", estack);
    iprintf(indent_size, " max_size: %zu\n", estack->max_size);
    iprintf(indent_size, " deque:\n");
    deque_debug_print_with_indent(estack->deque, print_value, indent_size + 2);
}
