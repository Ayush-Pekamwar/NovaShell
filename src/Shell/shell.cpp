#include "shell.h"

#include "../Commands/builtins.h"
#include "../Commands/external.h"
#include "../Parser/parser.h"
#include "../Utils/path.h"

#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>


// main REPL loop
void Shell::run() {
    while (true) {
        printPrompt();
        std::string input = readInput();
        std::vector<std::string> tokens = parser(input);
        if (!dispatch(tokens, input)) {
            return;
        }
    }
}

// main dispatch
bool Shell::dispatch(const std::vector<std::string> &tokens,
                     const std::string &input) {
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
