#!/bin/bash

EXE=${EXE:-./step}
EXE_ARGS=${EXE_ARGS:-}
MAKE_TARGET=${MAKE_TARGET:-default}

# It just works!
# - https://github.com/karekoho/valgrind-container
# - https://stackoverflow.com/questions/75292406/memchk-valgrind-reporting-inconsistent-results-in-different-docker-hosts

docker run --rm -ti \
    -v ./:/valgrind \
    step-valgrind:latest \
    "ulimit -n 1024; EXE=${EXE} EXE_ARGS=${EXE_ARGS} MAKE_TARGET=${MAKE_TARGET} ./scripts/valgrind_run.sh"
