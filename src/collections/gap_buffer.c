#include "collections/gap_buffer.h"

#include <malloc.h>
#include <stddef.h>
#include <string.h>

#include "mem.h"

#define DEFAULT_BUFFER_SIZE 64
#define DEFAULT_GAP_SIZE 16

struct GapBuffer {
    char *buffer;
    // Size of allocated buffer
    size_t size;
    // A total bytes occupied in buffer (string + gap)
    size_t length;
    // Index of left gap border (inclusive)
    size_t left;
    // Index of right gap border (inclusive)
    size_t right;
};

struct GapBuffer *gap_buffer_create_empty() {
    struct GapBuffer *gb = malloc(sizeof(struct GapBuffer));
    gb->buffer = NULL;
    gb->size = 0;
    gb->length = 0;
    gb->left = 0;
    gb->right = 0;
    return gb;
}

struct GapBuffer *gap_buffer_create() {
    struct GapBuffer *gb = gap_buffer_create_empty();
    gb->buffer = malloc(DEFAULT_BUFFER_SIZE + 1);
    gb->size = DEFAULT_BUFFER_SIZE;
    return gb;
}

struct GapBuffer *gap_buffer_create_from_string(const char *s) {
    struct GapBuffer *gb = gap_buffer_create();
    gap_buffer_insert(gb, 0, s);
    return gb;
}

void gap_buffer_destroy(struct GapBuffer *gb) {
    free(gb->buffer);
    gb->buffer = FREED_DUMMY;
    free(gb);
}

void _gap_buffer_inrease_size(struct GapBuffer *gb, size_t size) {
    while (gb->size <= size) {
        gb->size *= 2; // TODO: improve
    }

    gb->buffer = realloc(gb->buffer, gb->size);
}

void _gap_buffer_grow(struct GapBuffer *gb, size_t pos, size_t n) {
    size_t required_size = gb->length + n;
    if (required_size >= gb->size) {
        _gap_buffer_inrease_size(gb, required_size);
    }

    for (size_t i = gb->length + n; i > gb->length; i--) {
        gb->buffer[i] = gb->buffer[i - n];
    }

    for (size_t i = pos; i < pos + n; i++) {
        gb->buffer[i] = '_'; // TODO: remove?
    }

    gb->length += n;
    gb->left = pos;
    gb->right = gb->left + n;
}

void _gap_buffer_move_left(struct GapBuffer *gb, size_t pos) {
    while (gb->left > pos) {
        gb->left--;
        gb->right--;
        gb->buffer[gb->right + 1] = gb->buffer[gb->left];
        gb->buffer[gb->left] = '_'; // TODO: remove?
    }
}

void _gap_buffer_move_right(struct GapBuffer *gb, size_t pos) {
    while (gb->left < pos) {
        gb->left++;
        gb->right++;
        gb->buffer[gb->left - 1] = gb->buffer[gb->right];
        gb->buffer[gb->right] = '_'; // TODO: remove?
    }
}

void gap_buffer_move_gap(struct GapBuffer *gb, size_t pos) {
    if (pos < gb->left) {
        _gap_buffer_move_left(gb, pos);
    } else {
        _gap_buffer_move_right(gb, pos);
    }
}

void gap_buffer_insert(struct GapBuffer *gb, size_t pos, const char *s) {
    size_t n = strlen(s);

    if (n == 0) {
        return;
    }

    if (gb->left != pos) {
        gap_buffer_move_gap(gb, pos);
    }

    size_t i = 0;
    size_t current_pos = pos; // TODO: just use gb->left?

    while (i < n) {
        if (gb->left == gb->right) {
            _gap_buffer_grow(gb, current_pos, DEFAULT_GAP_SIZE);
        }

        gb->buffer[gb->left] = s[i];
        gb->left++;
        i++;
        current_pos++;
    }
}

void gap_buffer_delete(struct GapBuffer *gb, size_t pos) {
    if (pos + 1 == gb->size) {
        gap_buffer_move_gap(gb, pos);
        gb->right++;
        gb->buffer[gb->right] = '_'; // TODO: remove?
        return;
    }

    gap_buffer_move_gap(gb, pos + 1);
    gb->left--;
    gb->buffer[gb->left] = '_'; // TODO: remove?
}

void gap_buffer_print(const struct GapBuffer *gb) {
    size_t index = 0;

    while (index < gb->length) {
        if (index == gb->left) {
            index = gb->right + 1;
            if (index >= gb->length) {
                break;
            }
        }
        putc(gb->buffer[index], stdout);
        index++;
    }
}

size_t _gap_buffer_get_gap_size(const struct GapBuffer *gb) {
    return gb->size;
}

size_t _gap_buffer_get_real_position(const struct GapBuffer *gb, size_t pos) {
    if (pos < gb->left) {
        return pos;
    }
    return pos + _gap_buffer_get_gap_size(gb);
}

void gap_buffer_get_at(const struct GapBuffer *gb, size_t pos) {
    return gb->buffer[_gap_buffer_get_real_index(pos)];
}

void gap_buffer_set_at(struct GapBuffer *gb, size_t pos, char c) {
    gb->buffer[_gap_buffer_get_real_index(pos)] = c;
}

void gap_buffer_debug_print(const struct GapBuffer *gb) {
    printf("GapBuffer (0x%lx)\n", (size_t)gb);
    printf(" length: %zu\n", gb->length);
    printf(" size:   %zu\n", gb->size);
    printf(" left:   %zu\n", gb->left);
    printf(" right:  %zu\n", gb->right);
    printf(" data:   %s\n", gb->buffer);
}

// TODO: iterate
