# step

Simple Text Editor, Personalized

## Build

The only external dependency is `ncurses` (and `panels`, which is usually shipped with ncurses).

```bash
git clone https://github.com/sultanowskii/step
cd step
make
```

## Install

This command will install the compiled executable to `/usr/bin/` (you might want to run it with `sudo`)

```bash
make install
```

## Usage

```bash
> ./step
usage: ./step FILENAME
> ./step file.txt
```

### Modes

The idea is very similar to vim. There are normal (command) and insert modes.

#### All modes

These keys work in all modes:

| Key           | Action            |
|---------------|-------------------|
| ←, ↑, →, ↓    | Navigation keys   |

#### Normal (command) mode

This is where you would

| Key           | Action                       |
|---------------|------------------------------|
| `Ctrl+q`      | Quit                         |
| `Ctrl+s`      | Save                         |
| `u`           | Undo                         |
| `r`           | Redo                         |
| `g`           | Goto Beginning of File (BOF) |
| `G`           | Goto End of File (EOF)       |
| `i`           | Enter **Insert mode**        |

#### Insert mode

Basic text insertion mode - just like in most of text editors.

| Key               | Action                                       |
|-------------------|----------------------------------------------|
| `Ctrl+q` / `Esc`  | Exit Insert Mode (aka enter **Normal mode**) |
| `Backspace`/`Del` | Remove symbol                                |

## Features

- Undo/redo support - it might be simple and not as fancy as what modern text editors offer, but it works
- (As far as I know) doesn't have memory leaks

Not a feature itself, but worth mentioning: written fully in C, using `ncurses` for the TUI

## Limitations

- ASCII only
- Enormously large files aren't handled in any specific way, therefore some operations (as well as opening/saving) might be slow
- Currently, no copy/cut/paste support
- As some other TUIs, could work weirdly in small terminal windows - get a bigger one :)

## About

I've played with `ncurses` before, but I've always wanted to actually make an app using it. Well, I did it.
