#pragma once
#include <string>
#include <unordered_set>
#include <vector>

class Shell {
  public:
    void run();

  private:
    bool dispatch(const std::vector<std::string> &tokens,
                  const std::string &input);

    // Helper functions
    void printPrompt();
    std::string readInput();
};
