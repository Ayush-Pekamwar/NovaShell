#include "./parser.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

std::vector<std::string> parser(const std::string &input) {

    std::vector<std::string> tokens;
    //state
    std::string curToken;
    bool isSingleQuote = false;
    bool isDoubleQuote = false;
    bool isBackSlash = false;

    for(const char& c : input){
        if(isBackSlash == true){
            curToken += c;
            isBackSlash = false;
            continue;
        }
        // c is backslash and outside quotes then we can take next char as it is
        if(c=='\\' && !isSingleQuote && !isBackSlash){
            isBackSlash=true;
            continue;
        }
        if(c=='\"' && !isSingleQuote){
            isDoubleQuote^=1;
            continue;
        }
        if(c=='\'' && !isDoubleQuote){
            // toggle the state 
            isSingleQuote ^= 1;
            continue;
        }
        if(c == ' ' && !isSingleQuote && !isDoubleQuote && !isBackSlash){
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