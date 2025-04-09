#include "collections/bit_array.h"

#include <inttypes.h>
#include <malloc.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

struct BitArray {
    size_t   size;
    uint8_t *data;
};

struct BitArray *bit_array_create_empty(void) {
    struct BitArray *bit_array = malloc(sizeof(struct BitArray));
    bit_array->size = 0;
    bit_array->data = NULL;
    return bit_array;
}

struct BitArray *bit_array_create(size_t size) {
    size_t size_bytes = size / sizeof(uint8_t);
    if (size % sizeof(uint8_t) != 0) {
        size_bytes++;
    }
    struct BitArray *bit_array = bit_array_create_empty();
    bit_array->data = calloc(size_bytes, sizeof(uint8_t));
    bit_array->size = size_bytes * sizeof(uint8_t);
    return bit_array;
}

void bit_array_destroy(struct BitArray *bit_array) {
    free(bit_array->data);
    bit_array->data = NULL;
    bit_array->size = 0;
    free(bit_array);
}

bit bit_array_get_at(struct BitArray *bit_array, size_t index) {
    size_t byte_i = index / sizeof(uint8_t);
    size_t bit_i = index % sizeof(uint8_t);

    return ((bit_array->data[byte_i]) >> bit_i) & 1;
}

void bit_array_set_at(struct BitArray *bit_array, size_t index) {
    size_t byte_i = index / sizeof(uint8_t);
    size_t bit_i = index % sizeof(uint8_t);

    uint8_t new_value = (bit_array->data[byte_i]) | (1 << bit_i);
    bit_array->data[byte_i] = new_value;
}

void bit_array_unset_at(struct BitArray *bit_array, size_t index) {
    size_t byte_i = index / sizeof(uint8_t);
    size_t bit_i = index % sizeof(uint8_t);

    uint8_t new_value = (bit_array->data[byte_i]) & (~(1 << bit_i));
    bit_array->data[byte_i] = new_value;
}

bool bit_array_test_at(struct BitArray *bit_array, size_t index) {
    return bit_array_get_at(bit_array, index);
}

void bit_array_clear(struct BitArray *bit_array) {
    memset(bit_array->data, 0, bit_array->size);
}

void bit_array_flood(struct BitArray *bit_array) {
    memset(bit_array->data, UINT8_MAX, bit_array->size);
}
