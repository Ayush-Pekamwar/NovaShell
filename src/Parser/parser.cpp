#include "./parser.h"

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

Redirection parseRedirection(std::vector<std::string>& tokens){
    // this function takes tokens , removes redirection arguments
    // returns all required info about redirect in struct -> can be handled further before dispatch

    Redirection redirect;
    redirect.parseError = false;
    redirect.stdoutRedirect = false;
    redirect.stdoutFile = "";

    redirect.stderrRedirect = false;
    redirect.stderrFile = "";

    if(tokens.back() == ">" || tokens.back() == "1>" || tokens.back() == "2>"){
        // this case is basically where we want to redirect but user has not provided any file path,
        // throw back an error
        redirect.parseError = true;
        return redirect;
    } 

    for(int i=0; i<tokens.size()-1; i++){
        //stdout redirecting token found with file path in next token
        if(tokens[i] == ">" || tokens[i] == "1>"){
            redirect.stdoutRedirect = true;
            redirect.stdoutFile = tokens[i+1];
            // removing i and i+1 th element
            tokens.erase(tokens.begin()+i, tokens.begin()+i+2);
            break;
        }
        
        // stderr redirecting token found with file path in next token
        if (tokens[i] == "2>") {
            redirect.stderrRedirect = true;
            redirect.stderrFile = tokens[i + 1];
            // removing i and i+1 th element from tokens
            tokens.erase(tokens.begin()+i, tokens.begin()+i+2);
            break;
        }
    }

    return redirect;
}