#pragma once

#include <string>
#include <vector>
#include <unordered_set>

bool isBuiltin(const std::string& command);
bool executeBuiltin(const std::vector<std::string>& tokens);

bool executeEcho(const std::vector<std::string> &tokens);
bool executeType(const std::vector<std::string> &tokens);
bool executePwd();
bool executeCd(const std::string &path);
bool executeJobs();