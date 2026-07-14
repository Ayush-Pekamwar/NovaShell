#include "shell.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

void Shell::printPrompt() {
    std::cout << "$ ";
}

std::string Shell::readInput() {
    std::string input;
    std::getline(std::cin, input);
    return input;
}

std::vector<std::string> tokenize(const std::string &input) {
    std::istringstream iss(input);
    std::vector<std::string> tokens;
    std::string token;

    while (iss >> token) {
        tokens.push_back(token);
    }

    return tokens;
}

bool Shell::executeCommand(const std::vector<std::string> &tokens,
                           const std::string &input) {
    // returns true-> to keep executing the shell,
    // returns false -> to terminate the shell

    if (tokens.empty()) {
        return true;
    }

    if (tokens[0] == "exit") {
        return false;
    }
    if (tokens[0] == "echo") {
        for (int i = 1; i < tokens.size(); i++) {
            if (i > 1) {
                cout << ' ';
            }
            cout << tokens[i];
        }
        cout << endl;
        return true;
    }
    else {
        cout << input << ": command not found" << endl;
        return true;
    }
}

void Shell::run() {
    while (true) {
        printPrompt();
        std::string input = readInput();
        std::vector<std::string> tokens = tokenize(input);
        if (!executeCommand(tokens, input)) {
            return;
        }
    }
}
