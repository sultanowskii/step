#!/bin/bash

EXE=${EXE:-step}
EXE_ARGS=${EXE_ARGS:-}
MAKE_TARGET=${MAKE_TARGET:-default}

make "${MAKE_TARGET}"

valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --leak-resolution=high \
         ./${EXE} ${EXE_ARGS}
