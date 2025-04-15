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

The idea is very similar to vim. There are:

- Normal (command)
- Insert (the usual text editor experience)
- Visual (selection)

#### All modes

These keys work in all modes:

| Key           | Action            |
|---------------|-------------------|
| ←, ↑, →, ↓    | Navigation keys   |

#### Normal (command) mode

| Key           | Action                       |
|---------------|------------------------------|
| `Ctrl+q`      | Quit                         |
| `Ctrl+s`      | Save                         |
| `u`           | Undo                         |
| `r`           | Redo                         |
| `g`           | Goto Beginning of File (BOF) |
| `G`           | Goto End of File (EOF)       |
| `x`           | Remove one symbol            |
| `p`           | Insert clipboard             |
| `i`           | Enter **Insert mode**        |
| `v`           | Enter **Visual mode**        |

#### Insert mode

Basic text insertion mode - just like in most of text editors.

| Key               | Action                                          |
|-------------------|-------------------------------------------------|
| `Ctrl+q` / `Esc`  | Exit **Insert Mode** (= Enter **Normal mode**) |

#### Visual (select) mode

| Key              | Action                                         |
|------------------|------------------------------------------------|
| `c`              | Copy selected to clipboard                     |
| `x`              | Cut selected to clipboard                      |
| `Ctrl+q` / `Esc` | Exit **Visual Mode** (= Enter **Normal mode**) |

## Limitations

- ASCII only
- Enormously large files aren't handled in any specific way, therefore some operations (as well as opening/saving) might be slow
- As some other TUIs, could work weirdly in small terminal windows - get a bigger one :)
- Internal clipboard only (I'm NOT making the real clipboard support - that's one hell of a nightmare)

## About

The protracted pet ncurses project. Still like it quite a lot.
