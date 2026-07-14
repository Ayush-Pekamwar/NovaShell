#include <iostream>
#include <string>
using namespace std;

bool shouldTerminate(string &userInput) {
    return userInput == "exit";
}

int main() {
    // Flush after every std::cout / std:cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    while (1) {
        cout << "$ ";
        string userInput;
        getline(cin, userInput);

        if (shouldTerminate(userInput)) {
            return 0;
        }
        cout << userInput << ": command not found" << endl;
    }
}
