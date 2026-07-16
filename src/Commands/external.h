#pragma once
#include <string>
#include <vector>

bool executeExternalCommand(const std::string &executablePath,
                            const std::vector<std::string> &tokens);