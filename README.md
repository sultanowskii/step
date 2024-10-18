# step

Simple Text Editor, Personalized

## Build

The only external dependency is `ncurses` (and `panels`, usually is shipped with ncurses).

```bash
git clone https://github.com/sultanowskii/step
cd step
make
```

## Install

This command will install the compiled executable to `/usr/bin/`

```bash
make install
```

## Usage

```bash
> ./step
usage: ./step FILENAME
> ./step file.txt
```

### Key bindings

| Key bind      | Action          |
|---------------|-----------------|
| Ctrl+q        | Quit            |
| Ctrl+z        | Undo            |
| Ctrl+y        | Redo            |
| Ctrl+s        | Save            |
| ←, ↑, →, ↓    | Navigation keys |
| Backspace/Del | Remove symbol   |

## Features

- Undo/redo support - it might be simple and not as fancy as what IDEs offer, but it works
- (As far as I know) doesn't have memory leaks

Not a feature itself, but worth mentioning: written fully in C, using `ncurses` for the TUI

## Limitations

- ASCII only
- Enormously large files aren't handled in any specific way, therefore some operations (as well as opening/saving) might be slow
- Currently, no copy/cut/paste support
- As some other TUIs, could work weirdly in small terminal windows

## About

I've played with `ncurses` before, but I've always wanted to actually make an app using it. Well, I did it.
