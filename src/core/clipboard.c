#include "core/clipboard.h"

#include <malloc.h>
#include <stddef.h>
#include <string.h>

#define DEFAULT_CLIPBOARD_CAPACITY 128

struct Clipboard {
    char  *data;
    size_t capacity;
};

struct Clipboard *clipboard_create(void) {
    struct Clipboard *clipboard = malloc(sizeof(struct Clipboard));
    clipboard->data = malloc(DEFAULT_CLIPBOARD_CAPACITY);
    clipboard->capacity = DEFAULT_CLIPBOARD_CAPACITY;
    return clipboard;
}

void clipboard_destroy(struct Clipboard *clipboard) {
    free(clipboard->data);
    clipboard->data = NULL;
    clipboard->capacity = 0;
    free(clipboard);
}

const char *clipboard_get(struct Clipboard *clipboard) {
    return clipboard->data;
}

void clipboard_set(struct Clipboard *clipboard, const char *s, size_t length) {
    if (length >= clipboard->capacity) {
        size_t new_capacity = length + 1;
        clipboard->data = realloc(clipboard->data, new_capacity);
        clipboard->capacity = new_capacity;
    }

    strncpy(clipboard->data, s, length + 1);
}
