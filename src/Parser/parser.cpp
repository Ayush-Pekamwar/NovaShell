#include "./parser.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

std::vector<std::string> tokenize(const std::string &input) {
    std::istringstream iss(input);
    std::vector<std::string> tokens;
    std::string token;

    while (iss >> token) {
        tokens.push_back(token);
    }

    return tokens;
}