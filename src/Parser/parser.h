#pragma once

#include <string>
#include <vector>

std::vector<std::string> parser(const std::string &input);
// returns true if current command is background job, else returns false
bool parseBackgroundJob(std::vector<std::string>& tokens);