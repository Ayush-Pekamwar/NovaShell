#include "./parser.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

std::vector<std::string> parser(const std::string &input) {

    std::vector<std::string> tokens;
    std::string curToken;
    bool isSingleQuote = false;

    for(const char& c : input){
        if(c=='\''){
            // toggle the state 
            isSingleQuote ^= 1;
            continue;
        }

        if(c == ' ' && !isSingleQuote){
            if(!curToken.empty()) tokens.push_back(curToken);
            curToken.clear();
        }
        else{
            curToken += c;
        }
    }

    if(!curToken.empty()){
        tokens.push_back(curToken);
        curToken.clear();
    }

    return tokens;
}