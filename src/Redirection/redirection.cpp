#include "./redirection.h"
#include <vector>
#include <string>

#include <fcntl.h>    // open
#include <sys/stat.h> // file permissions
#include <unistd.h>   // dup, dup2, close

Redirection parseRedirection(std::vector<std::string> &tokens) {
    // this function takes tokens , removes redirection arguments
    // returns all required info about redirect in struct -> can be handled
    // further before dispatch

    Redirection redirect;

    if (tokens.empty()) {
        return redirect;
    }

    if (tokens.back() == ">" || tokens.back() == "1>" ||
        tokens.back() == "2>" || tokens.back() == ">>" ||
        tokens.back() == "1>>" || tokens.back() == "2>>") {
        // this case is basically where we want to redirect but user has not
        // provided any file path, throw back an error
        redirect.parseError = true;
        return redirect;
    }

    for (int i = 0; i < tokens.size() - 1; i++) {
        // stdout redirecting token found with file path in next token
        if (tokens[i] == ">" || tokens[i] == "1>" || tokens[i] == ">>" ||
            tokens[i] == "1>>") {
            redirect.stdoutRedirect.redirect = true;
            redirect.stdoutRedirect.file = tokens[i + 1];

            if (tokens[i] == ">>" || tokens[i] == "1>>") {
                redirect.stdoutRedirect.append = true;
            }

            // removing i and i+1 th element
            tokens.erase(tokens.begin() + i, tokens.begin() + i + 2);
            break;
        }

        // stderr redirecting token found with file path in next token
        if (tokens[i] == "2>" || tokens[i] == "2>>") {
            redirect.stderrRedirect.redirect = true;
            redirect.stderrRedirect.file = tokens[i + 1];

            if (tokens[i] == "2>>")
                redirect.stderrRedirect.append = true;

            // removing i and i+1 th element from tokens
            tokens.erase(tokens.begin() + i, tokens.begin() + i + 2);
            break;
        }
    }

    return redirect;
}

int setupRedirection(const StreamRedirect & stream, int targetFd) {
    int flags = O_WRONLY | O_CREAT;
    if (stream.append)  flags |= O_APPEND;
    else flags |= O_TRUNC;

    int savedFD = -1;
    if (stream.redirect == true) {
        // redirect stdout to new file descriptor
        int newStreamFD = open(stream.file.c_str(), flags, 0644);
        
        if (newStreamFD < 0) {
            perror("open");
            return -1;
        }
        
        savedFD = dup(targetFd);
        dup2(newStreamFD, targetFd);
        close(newStreamFD);
    }

    return savedFD;
}

int restoreRedirection(int savedFD , int targetFD){
    if(savedFD < 0) return 0;

    dup2(savedFD , targetFD);
    close(savedFD);

    return 1;
}
