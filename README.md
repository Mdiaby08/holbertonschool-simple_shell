# Simple Shell — hsh

A minimal UNIX command line interpreter written in C, built as part of the
Holberton School curriculum. It replicates the core behaviour of `/bin/sh`.

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
| `shell.h` | Header — all `#include`, `extern char **environ`, and function prototypes |
| `shell.c` | Entry point — main loop (prompt → read → execute → repeat) |
| `utils.c` | `remove_newline()`, `split_line()` — string utilities |
| `path.c` | `find_path()`, `get_path_value()`, `build_path()` — PATH resolution |
| `execute.c` | `execute_command()` — built-in detection, fork, execve, wait |
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
AUTHORS  execute.c  hsh  man_1_simple_shell  path.c  shell.c  shell.h  utils.c
$ ls
AUTHORS  execute.c  hsh  man_1_simple_shell  path.c  shell.c  shell.h  utils.c
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
| `exit` | Exits the shell with the last command's status |
| `env` | Prints all current environment variables, one per line |

Built-ins are handled directly by the shell process — no `fork` is created.

---

## How It Works

```
┌──────────────────────────────────────────────┐
│                  Main Loop                   │
│                                              │
│  1. isatty()  →  print "$ " only if          │
│     stdin is a terminal (not a pipe)         │
│                                              │
│  2. getline() →  reads user input            │
│     auto-allocates buffer of any size        │
│     returns -1 on EOF → shell exits          │
│                                              │
│  3. remove_newline()  →  strips '\n'         │
│                                              │
│  4. "exit" check  →  exit(last_status)       │
│                                              │
│  5. execute_command()                        │
│     split_line() → char **args via strtok    │
│     "env" → print environ[], no fork         │
│     args[0] has '/' → access() directly      │
│     else → find_path() searches PATH dirs    │
│                                              │
│  6. fork()  →  duplicates the process        │
│     child (pid==0) : execve() replaces image │
│     parent         : waitpid() + WEXITSTATUS │
│                                              │
│  7. Loop back to step 1                      │
└──────────────────────────────────────────────┘
```

---

## File Breakdown

### `shell.h`
Centralises all `#include` directives and function prototypes so every `.c`
file can access all functions without redeclaring them.
`extern char **environ` gives access to the global array of `"NAME=value"`
environment strings provided by the system.

### `shell.c`
Contains only the main loop.
`isatty(STDIN_FILENO)` detects whether stdin is a real terminal — if not
(pipe or script), no prompt is printed.
`getline` handles memory allocation automatically.
`remove_newline` strips the trailing `\n` left by `getline`.
`exit` is handled directly in the loop before `execute_command` is called,
so the shell process itself exits — not a child.

### `utils.c`
`remove_newline` uses `strcspn` to find the index of the first `\n` and
replaces it with `\0`.
`split_line` uses `strtok` to split on spaces and tabs, modifying the string
in place. The resulting array ends with `NULL`, which is required by `execve`.

### `path.c`
`get_path_value` scans `environ[]` entry by entry until it finds one starting
with `PATH=`, then returns a pointer to the value after the `=`.
`build_path` allocates and assembles the string `"dir/command"`.
`find_path` calls `build_path` for each directory in PATH and tests the
result with `access(X_OK)`, returning the first executable match found.

### `execute.c`
`env` is handled without `fork` — the shell process prints `environ[]`
directly and returns.
For all other commands: if `args[0]` contains `/` the path is tested
directly with `access`; otherwise `find_path` searches PATH.
`fork()` duplicates the process. The child (`pid == 0`) calls `execve` which
replaces its memory image with the new program. The parent waits with
`waitpid`. `WEXITSTATUS` extracts the child's exit code (0–255).

---

## Key System Calls

| Call | Purpose |
|------|---------|
| `getline` | Reads a full line from stdin, auto-manages buffer size |
| `isatty` | Checks whether a file descriptor is connected to a terminal |
| `strtok` | Splits a string into tokens using a delimiter set |
| `access` | Verifies a file exists and has execute permission |
| `fork` | Creates a child process identical to the parent |
| `execve` | Replaces the child process image with a new program |
| `waitpid` | Suspends the parent until the specified child terminates |
| `write` | Writes directly to a file descriptor, bypassing stdio buffers |
| `WEXITSTATUS` | Extracts the exit code from the status returned by waitpid |

---

## Authors

See the `AUTHORS` file.
