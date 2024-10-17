#include "tui/main.h"

#include <malloc.h>
#include <stdio.h>

#include "core/error_message.h"

char *error_message = NULL;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s FILENAME\n", argv[0]);
        return 1;
    }

    error_message = NULL;
    bool success = tui_main(argv[1]);

    if (!success) {
        fprintf(stderr, "error: %s\n", error_message);
        free(error_message);
        return 1;
    }

    return 0;
}
