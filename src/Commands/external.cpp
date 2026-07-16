#include "../Utils/path.h"

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <unordered_set>
#include <vector>

#ifdef _WIN32
const char PATH_DELIM = ';';
#else
const char PATH_DELIM = ':';
#endif

// Namespaces
namespace fs = std::filesystem;
using namespace std;

bool executeExternalCommand(const std::string &executablePath,
                                   const std::vector<std::string> &tokens) {
    pid_t pid = fork(); // returns pid of new process
    if (pid == 0) {
        // child process
        vector<char *> args = convertToCArgs(tokens);
        execv(executablePath.c_str(), args.data());
    }
    else {
        // parent process
        waitpid(pid, nullptr, 0);
    }

    return true;
}