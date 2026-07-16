#pragma once

#include <string>
#include <vector>
#include <unordered_set>

bool executeEcho(const std::vector<std::string> &tokens);
bool executeType(const std::vector<std::string> &tokens);
bool executePwd();
bool executeCd(const std::string &path);