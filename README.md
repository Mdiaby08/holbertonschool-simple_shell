# Simple Shell — hsh

A minimal UNIX command line interpreter written in C, built as part of the
Holberton School curriculum. It replicates the core behaviour of `/bin/sh`.

---

## Table of Contents

- [Description](#description)
- [Requirements](#requirements)
- [Files](#files)
- [Compilation](#compilation)
- [Usage](#usage)
- [Built-ins](#built-ins)
- [How It Works](#how-it-works)

---

## Description

`hsh` is a simple UNIX shell that:

- Displays a prompt and waits for the user to type a command
- Reads and parses the command line
- Searches for executables in the `PATH` environment variable
- Creates a child process with `fork()` and runs the command with `execve()`
- Waits for the child to finish before prompting again
- Handles both **interactive** and **non-interactive** modes
- Handles `EOF` (Ctrl+D) gracefully
- Reports errors to `stderr` in the same format as `/bin/sh`

---

## Requirements

- Ubuntu 20.04 LTS
- GCC with flags: `-Wall -Werror -Wextra -pedantic -std=gnu89`
- Betty coding style (checked with `betty-style.pl` and `betty-doc.pl`)
- No memory leaks
- No more than 5 functions per file
- All header files include-guarded

---

## Files

| File | Description |
|------|-------------|
| `shell.h` | Header file — all `#include`, `extern`, and function prototypes |
| `shell.c` | Entry point — main loop (Read → Parse → Execute → Repeat) |
| `prompt.c` | `display_prompt()` — prints `$ ` in interactive mode only |
| `read_line.c` | `read_line()` — reads one line from stdin via `getline` |
| `utils.c` | `remove_newline()`, `split_line()` — string utilities |
| `path.c` | `find_path()`, `get_path_value()`, `build_path()` — PATH resolution |
| `execute.c` | `execute_command()` — built-in detection, `fork`, `execve`, `wait` |
| `man_1_simple_shell` | Manual page for `hsh` |
| `AUTHORS` | List of project contributors |

---

## Compilation

```bash
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
```

---

## Usage

### Interactive mode

```bash
$ ./hsh
$ /bin/ls
AUTHORS  execute.c  hsh  man_1_simple_shell  path.c  prompt.c  read_line.c  shell.c  shell.h  utils.c
$ ls
AUTHORS  execute.c  hsh  man_1_simple_shell  path.c  prompt.c  read_line.c  shell.c  shell.h  utils.c
$ exit
```

### Non-interactive mode

```bash
$ echo "/bin/ls" | ./hsh
AUTHORS  execute.c  hsh  ...

$ cat commands.txt | ./hsh
...

$ echo "env" | ./hsh
USER=bob
PATH=/usr/local/bin:/usr/bin:/bin
...
```

### Error output (matches `/bin/sh` format)

```bash
$ echo "azerty" | ./hsh
./hsh: 1: azerty: not found
```

---

## Built-ins

| Command | Description |
|---------|-------------|
| `exit` | Exits the shell with status 0 |
| `env` | Prints all current environment variables, one per line |

Built-ins are handled directly by the shell process — no `fork` is created.

---

## How It Works

```
┌──────────────────────────────────────────────┐
│                  Main Loop                   │
│                                              │
│  1. display_prompt()  →  prints "$ "         │
│     (only if stdin is a terminal)            │
│                                              │
│  2. read_line()       →  reads user input    │
│     getline() + strip trailing newline       │
│     returns -1 on EOF → shell exits          │
│                                              │
│  3. split_line()      →  tokenizes input     │
│     strtok on spaces/tabs → char **args      │
│                                              │
│  4. Built-in check                           │
│     "exit" → exit(0)                         │
│     "env"  → print environ, continue         │
│                                              │
│  5. find_path()       →  resolve full path   │
│     if '/' in cmd → test directly            │
│     else → search each dir in PATH           │
│                                              │
│  6. fork()  →  create child process          │
│     child  : execve(path, args, environ)     │
│     parent : waitpid() → get exit status     │
│                                              │
│  7. Loop back to step 1                      │
└──────────────────────────────────────────────┘
```

### Key system calls used

| Call | Purpose |
|------|---------|
| `getline` | Read a full line from stdin, handles any length |
| `strtok` | Tokenize the line into an argument array |
| `access` | Check that a file exists and is executable |
| `fork` | Create a child process to run the command |
| `execve` | Replace the child's image with the new program |
| `waitpid` | Make the parent wait for the child to finish |
| `write` | Output to stdout/stderr (avoids buffering issues) |
