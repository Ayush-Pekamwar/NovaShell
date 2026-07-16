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

    for(const char& c : input){
        if(c=='\"' && !isSingleQuote){
            isDoubleQuote^=1;
            continue;
        }
        if(c=='\'' && !isDoubleQuote){
            // toggle the state 
            isSingleQuote ^= 1;
            continue;
        }

        if(c == ' ' && !isSingleQuote && !isDoubleQuote){
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