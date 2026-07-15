#pragma once
#include <string>
#include <unordered_set>
#include <vector>

class Shell {
  public:
    void run();

  private:
    std::unordered_set<std::string> shellCommands{"echo", "exit", "type"};
    bool dispatch(const std::vector<std::string> &tokens,
                  const std::string &input);

    // execute specific command
    bool executeEcho(const std::vector<std::string> &tokens);
    bool executeType(const std::vector<std::string> &tokens);
    bool executeExternalCommand(const std::string &executablePath,
                                const std::vector<std::string> &tokens);

    // Helper functions
    void printPrompt();
    std::string readInput();
    std::vector<std::string> tokenize(const std::string &input);
    std::string findExecutable(const std::string &command);
    std::vector<std::string> splitPath(const char *path_env);
};
