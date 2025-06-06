#include "collections/gap_buffer.h"

#include <assert.h>
#include <malloc.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "nonstd/fmt.h"
#include "nonstd/mem.h"

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

static void _gap_buffer_grow(struct GapBuffer *gb, size_t index, size_t n);

static inline struct GapBuffer *_gap_buffer_create_empty(void) {
    struct GapBuffer *gb = malloc(sizeof(struct GapBuffer));
    gb->buffer = NULL;
    gb->size = 0;
    gb->length = 0;
    gb->left = 0;
    gb->right = 0;
    return gb;
}

static inline struct GapBuffer *_gap_buffer_create(void) {
    struct GapBuffer *gb = _gap_buffer_create_empty();
    gb->buffer = calloc(DEFAULT_BUFFER_SIZE + 1, sizeof(char));
    gb->size = DEFAULT_BUFFER_SIZE;

    // otherwise `gap_buffer_create_from_string("")` would produce partially initialized buffer.
    _gap_buffer_grow(gb, 0, DEFAULT_BUFFER_SIZE / 2);
    // initial right index fix - to make it inclusive
    gb->right--;
    return gb;
}

struct GapBuffer *gap_buffer_create_from_string(const char *s) {
    struct GapBuffer *gb = _gap_buffer_create();
    gap_buffer_insert(gb, 0, s);
    return gb;
}

void gap_buffer_destroy(struct GapBuffer *gb) {
    free(gb->buffer);
    gb->buffer = FREED_DUMMY;
    free(gb);
}

static inline void _gap_buffer_inrease_size(struct GapBuffer *gb, size_t size) {
    while (gb->size <= size) {
        gb->size = grow_size(gb->size);
    }

    gb->buffer = realloc(gb->buffer, gb->size);
}

static inline void _gap_buffer_grow(struct GapBuffer *gb, size_t index, size_t n) {
    size_t length = gb->length;
    // number of symbols to move forward
    size_t to_move = length - index;

    size_t new_length = length + n;
    size_t required_size = new_length;
    if (required_size >= gb->size) {
        _gap_buffer_inrease_size(gb, required_size);
    }

    // move TO_MOVE symbols forward with offset of n
    for (size_t i = new_length; i > new_length - to_move; i--) {
        // hax: prevent unsigned >= 0 error.
        size_t index = i - 1;
        gb->buffer[index] = gb->buffer[index - n];
    }

    gb->length += n;
    gb->left = index;
    gb->right = gb->left + n;
}

static inline void _gap_buffer_move_left(struct GapBuffer *gb, size_t index) {
    while (gb->left > index) {
        gb->left--;
        gb->right--;
        gb->buffer[gb->right + 1] = gb->buffer[gb->left];
    }
}

static inline void _gap_buffer_move_right(struct GapBuffer *gb, size_t index) {
    while (gb->left < index) {
        gb->left++;
        gb->right++;
        gb->buffer[gb->left - 1] = gb->buffer[gb->right];
    }
}

static inline void _gap_buffer_move_gap(struct GapBuffer *gb, size_t index) {
    assert(gb->left <= gb->right);
    if (index < gb->left) {
        _gap_buffer_move_left(gb, index);
    } else {
        _gap_buffer_move_right(gb, index);
    }
}

void gap_buffer_move_gap(struct GapBuffer *gb, size_t index) {
    _gap_buffer_move_gap(gb, index);
}

static inline void _gap_buffer_insert(struct GapBuffer *gb, size_t index, const char *s, size_t length) {
    assert(gb->left <= gb->right);
    if (length == 0) {
        return;
    }

    if (gb->left != index) {
        _gap_buffer_move_gap(gb, index);
    }

    size_t i = 0;
    size_t current_index = index;

    while (i < length) {
        if (gb->left == gb->right) {
            _gap_buffer_grow(gb, current_index, DEFAULT_GAP_SIZE);
        }

        gb->buffer[gb->left] = s[i];
        gb->left++;
        i++;
        current_index++;
    }
}

void gap_buffer_insert(struct GapBuffer *gb, size_t index, const char *s) {
    _gap_buffer_insert(gb, index, s, strlen(s));
}

void gap_buffer_insert_symbol(struct GapBuffer *gb, size_t index, char symbol) {
    char buf[] = {symbol, '\0'};
    _gap_buffer_insert(gb, index, buf, 1);
}

static inline void _gap_buffer_delete(struct GapBuffer *gb, size_t index) {
    if (index + 1 == gb->size) {
        _gap_buffer_move_gap(gb, index);
        gb->right++;
        return;
    }

    _gap_buffer_move_gap(gb, index + 1);
    gb->left--;
}

void gap_buffer_delete(struct GapBuffer *gb, size_t index) {
    _gap_buffer_delete(gb, index);
}

void gap_buffer_delete_n(struct GapBuffer *gb, size_t index, size_t n) {
    for (size_t i = 0; i < n; i++) {
        _gap_buffer_delete(gb, index);
    }
}

static inline size_t _gap_buffer_get_gap_size(const struct GapBuffer *gb) {
    return gb->right - gb->left + 1;
}

static inline size_t _gap_buffer_get_real_index(const struct GapBuffer *gb, size_t index) {
    if (index < gb->left) {
        return index;
    }
    return index + _gap_buffer_get_gap_size(gb);
}

static inline size_t _gap_buffer_get_length(const struct GapBuffer *gb) {
    return gb->length - _gap_buffer_get_gap_size(gb);
}

size_t gap_buffer_get_length(const struct GapBuffer *gb) {
    return _gap_buffer_get_length(gb);
}

static inline char _gap_buffer_get_at(const struct GapBuffer *gb, size_t index) {
    return gb->buffer[_gap_buffer_get_real_index(gb, index)];
}

char gap_buffer_get_at(const struct GapBuffer *gb, size_t index) {
    return _gap_buffer_get_at(gb, index);
}

void gap_buffer_set_at(struct GapBuffer *gb, size_t index, char c) {
    gb->buffer[_gap_buffer_get_real_index(gb, index)] = c;
}

size_t gap_buffer_strncpy_from(struct GapBuffer *gb, char *buf, size_t starting_index, size_t n) {
    size_t written = 0;
    size_t buf_index = 0;
    size_t gb_index = starting_index;

    while (written < n && gb_index < _gap_buffer_get_length(gb)) {
        buf[buf_index] = _gap_buffer_get_at(gb, gb_index);
        buf_index++;
        gb_index++;
        written++;
    }
    buf[buf_index] = '\0';

    return written;
}

void gap_buffer_write_to_file(const struct GapBuffer *gb, FILE *f) {
    char  *left_part_start = gb->buffer;
    size_t left_part_length = gb->left;
    if (left_part_length > 0) {
        fwrite(left_part_start, sizeof(char), left_part_length, f);
    }

    char  *right_part_start = gb->buffer + gb->right + 1;
    size_t right_part_length = gb->length - gb->right - 1;
    if (right_part_length > 0) {
        fwrite(right_part_start, sizeof(char), right_part_length, f);
    }
}

void gap_buffer_print(const struct GapBuffer *gb) {
    gap_buffer_print_with_indent(gb, 0);
}

void gap_buffer_print_with_indent(const struct GapBuffer *gb, size_t indent_size) {
    size_t index = 0;

    print_indent(indent_size);

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

void gap_buffer_debug_print(const struct GapBuffer *gb) {
    gap_buffer_debug_print_with_indent(gb, 0);
}

void gap_buffer_debug_print_with_indent(const struct GapBuffer *gb, size_t indent_size) {
    iprintf(indent_size, "GapBuffer (0x%lx)\n", (size_t)gb);
    iprintf(indent_size, " length: %zu\n", gb->length);
    iprintf(indent_size, " size:   %zu\n", gb->size);
    iprintf(indent_size, " left:   %zu\n", gb->left);
    iprintf(indent_size, " right:  %zu\n", gb->right);
    iprintf(indent_size, " data:   %s\n", gb->buffer);
}
