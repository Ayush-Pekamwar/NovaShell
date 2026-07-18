#pragma once

#include <string>
#include <vector>

struct StreamRedirect{
    bool redirect = false;
    bool append = false;

    std::string file = "";
};

struct Redirection {
    bool parseError = false;

    StreamRedirect stdoutRedirect;
    StreamRedirect stderrRedirect;
};

std::vector<std::string> parser(const std::string &input);
Redirection parseRedirection(std::vector<std::string> &tokens);