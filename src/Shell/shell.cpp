#include "shell.h"

#include "../Commands/builtins.h"
#include "../Commands/external.h"
#include "../Parser/parser.h"
#include "../Utils/path.h"
#include "../Redirection/redirection.h"

#include <iostream>
#include <string>
#include <sys/stat.h>
#include <unordered_set>
#include <vector>

#include <fcntl.h> // open
#include <unistd.h> // dup, dup2, close
#include <sys/stat.h> // file permissions



// main REPL loop
void Shell::run() {
    while (true) {
        printPrompt();
        std::string input = readInput();
        std::vector<std::string> tokens = parser(input);
        Redirection redirect = parseRedirection(tokens);
        
        // handling parseError from redirection 
        if(redirect.parseError){
            std::cerr<<"NovaShell: Syntax Error: expected a filename after "<<tokens.back()<<std::endl;
            continue;
        }
        
       // redirection of stdout and stderr streams 
        int savedStdOutFD = -1;
        int savedStdErrFD = -1;
        if(redirect.stdoutRedirect.redirect){
            savedStdOutFD = setupRedirection(redirect.stdoutRedirect, STDOUT_FILENO);
        }
        if(redirect.stderrRedirect.redirect){
            savedStdErrFD = setupRedirection(redirect.stderrRedirect , STDERR_FILENO);
        }

        bool shouldContinue = dispatch(tokens, input);

        // restore stdout and stderr back to default
        restoreRedirection(savedStdOutFD,STDOUT_FILENO);
        restoreRedirection(savedStdErrFD, STDERR_FILENO);
        
        if (!shouldContinue) {
            return;
        }
    }
}

// main dispatch
bool Shell::dispatch(const std::vector<std::string> &tokens, const std::string &input) {
    // returns true-> to keep executing the shell,
    // returns false -> to terminate the shell

    if (tokens.empty()) {
        return true;
    }

    // if command is built-in shell command we would pass it to 
    // executeBuiltin() which would further call appropriate function
    if (isBuiltin(tokens[0])){
        return executeBuiltin(tokens);
    } 

    // checking if current command is executable external command or not
    // findExecutable would return path of executable if it exists in PATH variable if not , then it returns empty string
    std::string executablePath = findExecutable(tokens[0]);
    if (!executablePath.empty()) {
        return executeExternalCommand(executablePath, tokens);
    }

    // if neither condition satifies we can say command not found
    std::cout << input << ": not found\n";
    return true;
}

// Helper Functions
void Shell::printPrompt() {
    std::cout << "$ ";
}

std::string Shell::readInput() {
    std::string input;
    std::getline(std::cin, input);
    return input;
}
