# scripts directory

Execute all scripts from project root!

## `valgrind_run.sh`

Runs `valgrind` on an executable (`./ste` by default).

```bash
./scripts/valgrind_run.sh
```

To profile other executables, provide `EXE` (`ste` by default) and `MAKE_TARGET` (`default` by default) variables. Example:

```bash
EXE=playground_rope.elf MAKE_TARGET=playground-rope ./scripts/valgrind_run.sh
```

## `build_image.sh`

Builds local image (`ste-valgrind`) based on (`karek/valgrind`).

## `valgrind_docker_run.sh`

Same as `valgrind_run.sh`, but runs (and builds!) inside of a docker container. This one is preferred because `valgrind` just DOES NOT WORK quite often on host machine.
