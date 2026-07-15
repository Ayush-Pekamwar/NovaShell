# NovaShell
### (Project is under active development)
NovaShell is a Unix-style shell written in C++.  
It is built step by step, starting with the
basics of reading commands, handling builtins, finding executables on `PATH`,
and running external programs.

## Current Features

- Interactive prompt loop
- Basic whitespace tokenization
- Builtins: `exit`, `echo`, and `type`
- Executable lookup `(ex: ls, grep, curl, git, ssh, etc)` using the `PATH` environment variable
- External command execution with `fork()` and `execv()`

## Build and Run

```bash
cmake -B build -S .
cmake --build build
./build/shell
```

## In Progress

The shell is being expanded with support for navigation, quoting, redirection,
command and filename completion, programmable completion, background jobs,
pipelines, command history, history persistence, and parameter expansion.

Planned builtins and behavior include `pwd`, `cd`, `jobs`, `history`,
`declare`, richer completion handling, quoted executables, stdout/stderr
redirection, multi-command pipelines, and shell variable expansion.

## Current Project Structure

```text
src/
  main.cpp    # Program entry point
  shell.h     # Shell class interface
  shell.cpp   # Shell loop, builtins, command dispatch
  utils.h     # Utility declarations
  utils.cpp   # Utility implementations
```
