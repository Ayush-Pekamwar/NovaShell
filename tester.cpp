#include "./src/Parser/parser.cpp"
#include <bits/stdc++.h>

using namespace std;
int main(){
    string input; 
    getline(cin,input);
    vector<string> tokens = parser(input);
    Redirection redirect =  parseRedirection(tokens);
    cout<<"is stdout redirect: "<<redirect.stdoutRedirect<<endl;
    cout << "outputfile path: "<< redirect.outputFile << endl;

    cout<<"tokens size: "<<tokens.size()<<endl;
    for(auto& token : tokens){
        cout<<token<<endl;
    }
    return 0;
}
