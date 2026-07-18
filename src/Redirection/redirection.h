#pragma once

#include <string>
#include <vector>

struct StreamRedirect {
    bool redirect = false;
    bool append = false;

    std::string file = "";
};

struct Redirection {
    bool parseError = false;

    StreamRedirect stdoutRedirect;
    StreamRedirect stderrRedirect;
};

Redirection parseRedirection(std::vector<std::string> &tokens);
int setupRedirection(const StreamRedirect &, int targetFd);
int restoreRedirection(int savedFd, int targetFd);