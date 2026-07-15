#include "shell.h"
#include "utils.cpp"

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <unordered_set>
#include <vector>

#ifdef _WIN32
const char PATH_DELIM = ';';
#else
const char PATH_DELIM = ':';
#endif

namespace fs = std::filesystem;
using namespace std;

// organizing from higher level to lower level
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

void Shell::printPrompt() {
    std::cout << "$ ";
}

std::string Shell::readInput() {
    std::string input;
    std::getline(std::cin, input);
    return input;
}

std::vector<std::string> Shell::tokenize(const std::string &input) {
    std::istringstream iss(input);
    std::vector<std::string> tokens;
    std::string token;

    while (iss >> token) {
        tokens.push_back(token);
    }

    return tokens;
}

bool Shell::dispatch(const std::vector<std::string> &tokens,
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
        return executeEcho(tokens);
    }
    if (tokens[0] == "type") {
        return executeType(tokens);
    }

    string executablePath = findExecutable(tokens[0]);
    if (!executablePath.empty()) {
        executeExternalCommand(executablePath, tokens);
        return true;
    }
    else {
        cout << input << ": not found" << endl;
        return true;
    }
}

bool Shell::executeExternalCommand(const std::string &executablePath,
                                   const std::vector<std::string> &tokens) {
    pid_t pid = fork(); // returns pid of new process
    if (pid == 0) {
        // child process
        vector<char *> args = convertToCArgs(tokens);
        execv(executablePath.c_str(), args.data());
    }
    else {
        // parent process
        waitpid(pid, nullptr, 0);
    }

    return true;
}

bool Shell::executeEcho(const std::vector<std::string> &tokens) {
    for (int i = 1; i < tokens.size(); i++) {
        if (i > 1) {
            cout << ' ';
        }
        cout << tokens[i];
    }
    cout << endl;
    return true;
}

bool Shell::executeType(const std::vector<std::string> &tokens) {
    for (int i = 1; i < tokens.size(); i++) {
        if (shellCommands.find(tokens[i]) != shellCommands.end()) {
            cout << tokens[i] << " is a shell builtin" << endl;
        }
        else {
            string path = findExecutable(tokens[i]);
            if (!path.empty()) {
                cout << tokens[i] << " is " << path << endl;
            }
            else {
                cout << tokens[i] << ": not found" << endl;
            }
        }
    }

    return true;
}

string Shell::findExecutable(const string &command) {
    const char *path_env = std::getenv("PATH");
    if (path_env == nullptr) {
        return "";
    }

    vector<string> paths = splitPath(path_env);

    for (auto &directory : paths) {
        fs::path executablePath = fs::path(directory) / command;
        if (fs::is_regular_file(executablePath)) {
            if (access(executablePath.c_str(), X_OK) == 0) {
                return executablePath.string();
            }
        }
    }

    return "";
}

std::vector<std::string> Shell::splitPath(const char *path_env) {
    std::istringstream iss(path_env);
    vector<string> tokens;
    string token;

    while (getline(iss, token, PATH_DELIM)) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }

    return tokens;
}
