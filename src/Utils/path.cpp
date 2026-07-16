#include "path.h"

#include <filesystem>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

#ifdef _WIN32
const char PATH_DELIM = ';';
#else
const char PATH_DELIM = ':';
#endif

using namespace std;
namespace fs = std::filesystem;


std::vector<char *> convertToCArgs(std::vector<std::string> const &tokens) {
    std::vector<char *> args;

    // Pre-allocate memory for performance (size + 1 for the null terminator)
    args.reserve(tokens.size() + 1);

    for (auto &token : tokens) {
        args.push_back(const_cast<char *>(token.c_str()));
    }

    // POSIX system calls REQUIRE the array to end with a null pointer
    args.push_back(nullptr);

    return args;
}

string findExecutable(const string &command) {
    const char *path_env = std::getenv("PATH");
    if (path_env == nullptr) {
        return "";
    }

    vector<string> paths = splitPath(path_env);

    for (auto &directory : paths) {
        fs::path executablePath = fs::path(directory) / command;
        if (fs::is_regular_file(executablePath)) {
            if (access(executablePath.c_str(), X_OK) == 0) {
                return executablePath.string();
            }
        }
    }

    return "";
}

std::vector<std::string> splitPath(const char *path_env) {
    std::istringstream iss(path_env);
    vector<string> tokens;
    string token;

    while (getline(iss, token, PATH_DELIM)) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }

    return tokens;
}
