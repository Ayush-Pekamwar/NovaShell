#pragma once
#include <string>
#include <vector>

class Shell {
  public:
    void run();

  private:
    std::string readInput();
    void printPrompt();
    bool executeCommand(const std::vector<std::string> &tokens,
                        const std::string &input);
};
