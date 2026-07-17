#pragma once

#include <string>
#include <vector>

struct Redirection {
    bool stdoutRedirect = false;
    std::string outputFile = "";
};

std::vector<std::string> parser(const std::string &input);
Redirection parseRedirection(std::vector<std::string> &tokens);