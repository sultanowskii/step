#include "collections/gap_buffer.h"

#include <malloc.h>
#include <stddef.h>
#include <string.h>

#include "math.h"
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
    gb->buffer = calloc(DEFAULT_BUFFER_SIZE + 1, sizeof(char));
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
    printf("grow(%zu, %zu)\n", pos, n);
    size_t length = gb->length;
    // number of symbols to move forward
    size_t to_move = length - pos;

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

    for (size_t i = pos; i < pos + n; i++) {
        gb->buffer[i] = '_'; // TODO: remove?
    }

    gb->length += n;
    gb->left = pos;
    gb->right = gb->left + n - 1;
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
        gap_buffer_debug_print(gb);
        if (gb->left > gb->right) {
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

void gap_buffer_delete_n(struct GapBuffer *gb, size_t pos, size_t n) {
    for (size_t i = 0; i < n; i++) {
        gap_buffer_delete(gb, pos);
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

char gap_buffer_get_at(const struct GapBuffer *gb, size_t pos) {
    return gb->buffer[_gap_buffer_get_real_position(gb, pos)];
}

void gap_buffer_set_at(struct GapBuffer *gb, size_t pos, char c) {
    gb->buffer[_gap_buffer_get_real_position(gb, pos)] = c;
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
