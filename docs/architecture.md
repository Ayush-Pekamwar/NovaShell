# NovaShell Architecture

NovaShell is organized as a small set of modules around one main flow:

```text
main()
  -> Shell::run()
    -> readInput()
    -> tokens = parse(input)
    -> parseRedirection(tokens)

    -> handles redirection logic

    -> dispatch(tokens, input)
      -> builtin command
      -> external command
      -> command-not-found output
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

- `Shell::run()`: Repeatedly prints the prompt, reads input, tokenizes it, after tokenzing performs redirection from information received from `parseRedirection()` function and dispatches the command. Stops when `dispatch()` returns `false`.
- `Shell::dispatch(tokens, input)`: Decides how to handle a command:
  builtins go to `executeBuiltin()`, executable files go to
  `executeExternalCommand()`, and unknown commands print `not found`.
- `Shell::printPrompt()`: Writes `$ ` before each command.
- `Shell::readInput()`: Reads a full input line using `std::getline()`.

## Parser Module

### `src/Parser/parser.h` / `src/Parser/parser.cpp`

- `parse(input)`: Splits the input line into whitespace-separated tokens.
Handles single quotes `'` double quotes `"` and backslashes `\` properly 
- `parseRedirection(tokens)` : takes the tokens and handles all these commands `> , 1> , 2>, 1>> , >> , 2>> `.  to know whether stdout or stderr stream needs to be redirected. This function returns a struct (Redirection) which consists of the following information :
1. is current input command tokens valid or not, if not we return with a parseError flag set
2. consists of `StreamRedirect` struct for stdout and stderr which basically tells whether the stream is to be redirected? what is is file path? and do we have to append or overwrite to this file path?
File path not existing error is handling in run() itself;

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
