#!/bin/bash

# It just works!
# - https://github.com/karekoho/valgrind-container
# - https://stackoverflow.com/questions/75292406/memchk-valgrind-reporting-inconsistent-results-in-different-docker-hosts

docker run --rm -tiv ./:/valgrind karek/valgrind:latest bash -c "ulimit -n 1024 && ./valgrind_run.sh"
