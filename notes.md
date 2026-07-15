# General things
```
- convert string to char* by using myStr.c_str();
```

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

