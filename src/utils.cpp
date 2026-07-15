#include <string>
#include <vector>

// Note: We pass the vector by reference (not const) because we need modifiable
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