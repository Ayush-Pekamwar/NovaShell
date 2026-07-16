#pragma once

#include <string>
#include <vector>

std::vector<std::string> splitPath(const char *pathEnv);
std::string findExecutable(const std::string &command);

//helper
std::vector<char *> convertToCArgs(const std::vector<std::string> &tokens);