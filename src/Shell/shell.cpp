#include "shell.h"

#include "../Commands/builtins.h"
#include "../Commands/external.h"
#include "../Parser/parser.h"
#include "../Utils/path.h"

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
        
        int savedStdOutFD = -1;
        int newStdoutFileFD = -1;
        
        //handling stdout redirection
        if (redirect.stdoutRedirect == true) {
            // redirect stdout to new file descriptor
            newStdoutFileFD = open(redirect.stdoutFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            
            if (newStdoutFileFD < 0) {
                perror("open");
                continue;
            }
            
            savedStdOutFD = dup(STDOUT_FILENO);
            dup2(newStdoutFileFD, STDOUT_FILENO);
            close(newStdoutFileFD);
        }
        
        int savedStdErrFD = -1;
        int newStdErrFileFD = -1;

        //handling stderr redirection
        if(redirect.stderrRedirect == true){
            newStdErrFileFD = open(redirect.stderrFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            
            if(newStdErrFileFD < 0){
                perror("open");
                continue;
            }
            
            savedStdErrFD = dup(STDERR_FILENO);
            dup2(newStdErrFileFD, STDERR_FILENO);
            close(newStdErrFileFD);
        }
        

        bool shouldContinue = dispatch(tokens, input);

        // restore stdout 
        if(redirect.stdoutRedirect == true){
            dup2(savedStdOutFD, STDOUT_FILENO);
            close(savedStdOutFD);
        }
        // restore stderr
        if(redirect.stderrRedirect == true){
            dup2(savedStdErrFD, STDERR_FILENO);
            close(savedStdErrFD);
        }

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
