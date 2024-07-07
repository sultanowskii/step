#!/bin/bash

EXE=${EXE:-ste}
MAKE_TARGET=${MAKE_TARGET:-default}

make "${MAKE_TARGET}"

valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --leak-resolution=high \
         "./${EXE}"
