#!/bin/bash

EXE=${EXE:-./ste}
MAKE_TARGET=${MAKE_TARGET:-default}

# It just works!
# - https://github.com/karekoho/valgrind-container
# - https://stackoverflow.com/questions/75292406/memchk-valgrind-reporting-inconsistent-results-in-different-docker-hosts

docker run --rm -tiv \
    ./:/valgrind karek/valgrind:latest \
    bash -c "ulimit -n 1024; EXE=${EXE} MAKE_TARGET=${MAKE_TARGET} ./scripts/valgrind_run.sh"
