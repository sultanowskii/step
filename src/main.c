#include "tui/main.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
    // TODO: proper argument handling?
    if (argc != 2) {
        printf("usage: %s FILENAME\n", argv[0]);
        return 1;
    }

    tui_main(argv[1]);
    return 0;
}
