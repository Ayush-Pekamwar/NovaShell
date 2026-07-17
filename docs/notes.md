# General things
- `#include` directive is just copy-paste for .h and .cpp
- file.h is header file used for declaration and saying that hey here this functions are declared and file.cpp contains actual implementation 
- we must only include header files in our any .cpp file as if we do #include file.cpp then our code for file.cpp would be copied and pasted in our current main.cpp file, and when we build it with cmake the code of file.cpp gets compiles twice.
- solution? just use header files for declaration and inclusion 
main.cpp , file.cpp  -----+----> Linker ----> Executable

- now if main.cpp contains file.h then we just copy function declarations not there implementations thus avoid multiple compiled copies of same code (multiple copies of same compiled code can cause error) and freely use functions in file.cpp without including it directly in our main.cpp


- convert string to char* by using `myStr.c_str()`;



# Learnings Stages wise

# Run executable stage
- `fork()` -> creates a child process and returns its pid
- if i am inside child process i can use if(pid==0) to check whether it is a child process
or just use `getpid()` to get pid of current process and compare it with the one you got from the fork() return
- `waitpid(childpid, nullptr, 0)` -> current process which executes this function waits for childpid to end 
and it doesnt care about what it returns (2nd arg nullptr) 
- `execvp( arg1 , char*[] args)` -> arg1 can be executable command name or path to it, if you provide just the name then it searches PATH env variable to find this executable, or you can directly pass the path to executable , second argument is args passed to that executable as it is. this function replaces current process with executable program passed. that why we create the child process so we dont lose our main process of the shell
- `execvc((const char *) "/path/to/executable"  , args)` -> only difference between this and execvp() is that this function strictly takes the path instead of just command name


## Implementations:
- created executeExternalCommand() function which is called if we find executable for the first token entered in the shell at prompt
- arguments passed to this function are executablePath of first token, tokens list
- this function creates a child process using fork() and then converts this child process to executable which we wanted to execute 
- for implementation we had to get char*[] from tokens using a util function
- as child process (new exe) is done working we resume parent which was waiting and continue our shell


# Navigations

## pwd builtin
- `executePwd()` function implemented
- simple implementation just used filesystem library in c++ to get current working directory and printed it

## cd builtin
- `chdir(newPath.c_str())` already handles both absolute and relative paths, so i just need to check if the path exists or not using file systems lib functions ie. exists() and is_directory()
- just for `~` alias for HOME directory, we just replace `~` by home directory path extracted from HOME env variable and then pass the new target string path to same function

# Output Redirection (> / 1>)

Goal: Redirect stdout (FD 1) from the terminal to a file.

Steps
1. Open/Create the file

int fileFd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);

* O_WRONLY → open for writing
* O_CREAT → create if it doesn’t exist
* O_TRUNC → overwrite existing contents
* 0644 → rw-r--r--

2. Save current stdout

int savedStdout = dup(STDOUT_FILENO);

3. Redirect stdout to the file

dup2(fileFd, STDOUT_FILENO);

Now all std::cout, printf(), and write(STDOUT_FILENO, ...) output goes to the file.

4. Close the original file descriptor

close(fileFd);

Safe because dup2() already made STDOUT_FILENO refer to the file.

5. Execute the command

dispatch(...);

6. Restore stdout

dup2(savedStdout, STDOUT_FILENO);
close(savedStdout);

⸻

File Descriptor Flow

Before:

FD 1 ──► Terminal

After open():

FD 1 ──► Terminal
FD 3 ──► output.txt

After dup2(fileFd, STDOUT_FILENO):

FD 1 ──► output.txt
FD 3 ──► output.txt

After close(fileFd):

FD 1 ──► output.txt

After restoring:

FD 1 ──► Terminal

Key APIs

* open() → Open/Create file
* dup() → Duplicate a file descriptor (backup)
* dup2(oldfd, newfd) → Make newfd refer to the same file as oldfd
* close() → Close a file descriptor to avoid leaks
