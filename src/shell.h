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
};
