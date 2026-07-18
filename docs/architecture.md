# NovaShell Architecture

NovaShell is organized as a small set of modules around one main flow:

```text
main()
  -> Shell::run()
    -> readInput()
    -> tokens = parse(input)
    -> redirect = parseRedirection(tokens)
    -> setupRedirection(...)
    -> dispatch(tokens, input)
      -> builtin command
      -> external command
      -> command-not-found output
    -> restoreRedirection(...)
```

## File Layout

```text
src/
  main.cpp
  Shell/
    shell.h
    shell.cpp
  Parser/
    parser.h
    parser.cpp
  Redirection/
    redirection.h
    redirection.cpp
  Commands/
    builtins.h
    builtins.cpp
    external.h
    external.cpp
  Utils/
    path.h
    path.cpp
```

## Entry Point

### `src/main.cpp`

- `main()`: Configures unbuffered `stdout`/`stderr`, creates a `Shell`
  instance, and starts the shell loop with `shell.run()`.

## Shell Module

### `src/Shell/shell.h`

- `Shell`: Owns the main REPL behavior.
- `run()`: Public method used by `main()` to start the shell.
- `dispatch(tokens, input)`: Private command router.
- `printPrompt()`: Prints the shell prompt.
- `readInput()`: Reads one line from standard input.

### `src/Shell/shell.cpp`

- `Shell::run()`: Repeatedly prints the prompt, reads input, tokenizes it,
  parses redirection information, sets up redirected streams, dispatches the
  command, restores the original streams, and stops when `dispatch()` returns
  `false`.
- `Shell::dispatch(tokens, input)`: Decides how to handle a command:
  builtins go to `executeBuiltin()`, executable files go to
  `executeExternalCommand()`, and unknown commands print `not found`.
- `Shell::printPrompt()`: Writes `$ ` before each command.
- `Shell::readInput()`: Reads a full input line using `std::getline()`.

## Parser Module

### `src/Parser/parser.h` / `src/Parser/parser.cpp`

- `parse(input)`: Splits the input line into whitespace-separated tokens.
  Handles single quotes `'`, double quotes `"`, and backslashes `\`.

## Redirection Module

### `src/Redirection/redirection.h` / `src/Redirection/redirection.cpp`

- `StreamRedirect`: Stores redirection details for one stream: whether it
  should redirect, whether it should append, and the target file path.
- `Redirection`: Stores the full redirection parse result, including a
  `parseError` flag plus separate stdout and stderr redirect settings.
- `parseRedirection(tokens)`: Scans tokens for redirection operators:
  `>`, `1>`, `2>`, `>>`, `1>>`, and `2>>`. It records the target file,
  detects missing filenames, and removes the redirection operator and filename
  from `tokens` before command dispatch.
- `setupRedirection(stream, targetFd)`: Opens the target file, saves the
  original file descriptor, and redirects `targetFd` to the file using `dup2()`.
  This logic now lives in the redirection module instead of directly inside
  `Shell::run()`.
- `restoreRedirection(savedFd, targetFd)`: Restores a previously redirected
  stream back to its original file descriptor after the command finishes.

## Builtin Commands Module

### `src/Commands/builtins.h` / `src/Commands/builtins.cpp`

- `shellCommands`: Stores the names of supported builtins:
  `echo`, `pwd`, `exit`, `type`, and `cd`.
- `isBuiltin(command)`: Returns `true` when a command name is a shell builtin.
- `executeBuiltin(tokens)`: Routes builtin commands to their specific
  implementation and returns whether the shell should continue running.
- `executeEcho(tokens)`: Prints all arguments after `echo`, separated by spaces.
- `executeType(tokens)`: Reports whether each argument is a shell builtin or an
  executable found on `PATH`.
- `executePwd()`: Prints the current working directory.
- `executeCd(path)`: Changes the current working directory. Supports normal
  paths and `~` expansion using the `HOME` environment variable.

## External Commands Module

### `src/Commands/external.h` / `src/Commands/external.cpp`

- `executeExternalCommand(executablePath, tokens)`: Runs an external program by
  forking the shell process, converting tokens into C-style arguments, calling
  `execv()` in the child process, and waiting in the parent process.

## Utilities Module

### `src/Utils/path.h` / `src/Utils/path.cpp`

- `splitPath(pathEnv)`: Splits the `PATH` environment variable into individual
  directories.
- `findExecutable(command)`: Searches each directory in `PATH` for an
  executable file matching the command name.
- `convertToCArgs(tokens)`: Converts `std::vector<std::string>` into the
  null-terminated `char*` array required by POSIX functions like `execv()`.

## Build

### `CMakeLists.txt`

- Collects all source and header files under `src/`.
- Builds the `shell` executable using C++23.
- Links against `readline`.
