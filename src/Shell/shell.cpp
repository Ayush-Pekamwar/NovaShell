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
        std::vector<std::string> tokens = tokenize(input);
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

    if (tokens[0] == "exit") {
        // terminate the shell
        return false;
    }
    if (tokens[0] == "echo") {
        return executeEcho(tokens);
    }
    if (tokens[0] == "type") {
        return executeType(tokens);
    }
    if (tokens[0] == "pwd") {
        return executePwd();
    }
    if (tokens[0] == "cd") {
        if (tokens.size() > 1)
            executeCd(tokens[1]);
        return true;
    }

    std::string executablePath = findExecutable(tokens[0]);
    if (!executablePath.empty()) {
        return executeExternalCommand(executablePath, tokens);
    }
    else {
        std::cout << input << ": not found" << std::endl;
        return true;
    }
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
