#pragma once
#include <string>
#include <unordered_set>
#include <vector>

class Shell {
  public:
    void run();

  private:
    std::unordered_set<std::string> shellCommands{"echo", "exit", "type"};
    std::string readInput();
    void printPrompt();
    bool executeCommand(const std::vector<std::string> &tokens,
                        const std::string &input);

    // execute specific command
    bool executeType(const std::vector<std::string> &tokens);
    bool executeEcho(const std::vector<std::string> &tokens);

    // Helper functions
    std::vector<std::string> tokenize(const std::string &input);
    std::string findExecutable(const std::string &command);
    std::vector<std::string> splitPath(const char *path_env);
};
