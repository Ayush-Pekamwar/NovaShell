#include "../Utils/path.h"

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <sstream>
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

// built-in command functions
std::unordered_set<std::string> shellCommands{"echo", "pwd", "exit", "type","cd"};

bool executeEcho(const std::vector<std::string> &tokens) {
    for (int i = 1; i < tokens.size(); i++) {
        if (i > 1) {
            cout << ' ';
        }
        cout << tokens[i];
    }
    cout << endl;
    return true;
}

bool executeType(const std::vector<std::string> &tokens) {
    for (int i = 1; i < tokens.size(); i++) {
        if (shellCommands.find(tokens[i]) != shellCommands.end()) {
            cout << tokens[i] << " is a shell builtin" << endl;
        }
        else {
            string path = findExecutable(tokens[i]);
            if (!path.empty()) {
                cout << tokens[i] << " is " << path << endl;
            }
            else {
                cout << tokens[i] << ": not found" << endl;
            }
        }
    }

    return true;
}

bool executePwd() {
    try {
        fs::path cwd = fs::current_path();
        cout << cwd.string() << endl;
    } catch (const fs::filesystem_error &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return true;
}

bool executeCd(const std::string &newPath) {
    if (newPath.empty())
        return true;

    const char *HOME = std::getenv("HOME");
    std::string HOME_str(HOME);
    std::string target;

    // Home directory alias ~
    if (newPath[0] == '~') {
        target = HOME_str;
        target += newPath.substr(1);
    }
    else {
        target = newPath;
    }

    if (fs::exists(target) && fs::is_directory(target)) {
        if (chdir(target.c_str()) != 0) {
            cout << "internal error cannot change the directory" << endl;
        }
    }
    else {
        cout << "cd: " << target << ": No such file or directory" << endl;
    }

    return true;
}
