#pragma once

#include <string>
#include <vector>

struct Redirection {
    bool parseError = false;

    bool stdoutRedirect = false;
    std::string stdoutFile = "";

    bool stderrRedirect = false;
    std::string stderrFile = "";
};

std::vector<std::string> parser(const std::string &input);
Redirection parseRedirection(std::vector<std::string> &tokens);