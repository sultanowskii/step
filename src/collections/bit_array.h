#pragma once

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

struct BitArray;

typedef uint8_t bit;

struct BitArray *bit_array_create(size_t bytes);
void             bit_array_destroy(struct BitArray *bit_array);

bit  bit_array_get_at(struct BitArray *bit_array, size_t index);
bool bit_array_test_at(struct BitArray *bit_array, size_t index);

void bit_array_set_at(struct BitArray *bit_array, size_t index);
void bit_array_unset_at(struct BitArray *bit_array, size_t index);

void bit_array_clear(struct BitArray *bit_array);
void bit_array_flood(struct BitArray *bit_array);
