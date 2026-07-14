#include <iostream>
#include <string>
using namespace std;
int main()
{
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (1)
  {
    cout << "$ ";
    string userInput;
    getline(cin, userInput);
    cout << userInput << ": command not found" << endl;
  }
}
