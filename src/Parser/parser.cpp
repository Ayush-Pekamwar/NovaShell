#include "./parser.h"
#include "../Redirection/redirection.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

std::vector<std::string> parser(const std::string &input) {

    std::vector<std::string> tokens;
    //state
    std::string currentToken;
    bool isSingleQuote = false;
    bool isDoubleQuote = false;
    bool escapeNext = false;

    for(const char& c : input){
        if(escapeNext){
            currentToken += c;
            escapeNext = false;
            continue;
        }
        // c is backslash and outside quotes then we can take next char as it is
        if(c=='\\' && !isSingleQuote){
            escapeNext=true;
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
        if(c == ' ' && !isSingleQuote && !isDoubleQuote && !escapeNext){
            if(!currentToken.empty()) tokens.push_back(currentToken);
            currentToken.clear();
        }
        else{
            currentToken += c;
        }
    }

    if(!currentToken.empty()){
        tokens.push_back(currentToken);
        currentToken.clear();
    }

    return tokens;
}

bool parseBackgroundJob(std::vector<std::string>& tokens){
    if(!tokens.empty() && tokens.back() == "&"){
        tokens.pop_back();
        return true;
    }

    return false;
}