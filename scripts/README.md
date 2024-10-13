# scripts directory

Execute all scripts from the project root!

## [valgrind_run.sh](valgrind_run.sh)

Runs `valgrind` on an executable (`./step` by default).

```bash
./scripts/valgrind_run.sh
```

To profile other executables, provide `EXE` (`step` by default) and `MAKE_TARGET` (`default` by default) variables. Example:

```bash
EXE=playground_rope.elf MAKE_TARGET=playground-rope ./scripts/valgrind_run.sh
```

## [build_image.sh](build_image.sh)

Builds local image (`step-valgrind`) based on (`karek/valgrind`).

## [valgrind_docker_run.sh](valgrind_docker_run.sh)

Same as `valgrind_run.sh`, but runs (and builds app) inside of a docker container.

This one is preferred because `valgrind` just DOES NOT WORK quite often on the host machine.

```bash
EXE=step EXE_ARGS=Makefile MAKE_TARGET=build ./scripts/valgrind_docker_run.sh
```
