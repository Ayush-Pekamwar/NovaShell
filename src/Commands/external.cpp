#include "../Utils/path.h"
#include "../Jobs/jobs.h"

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <unordered_set>
#include <vector>

#ifdef _WIN32
const char PATH_DELIM = ';';
#else
const char PATH_DELIM = ':';
#endif

// Namespaces
namespace fs = std::filesystem;
using namespace std;

bool executeExternalCommand(const std::string &executablePath,
                            const std::vector<std::string> &tokens,
                            bool isBackgroundJob) {
    pid_t pid = fork(); // returns pid of new process
    if (pid == 0) {
        // child process
        vector<char *> args = convertToCArgs(tokens);
        execv(executablePath.c_str(), args.data());

        perror("execv");
        _exit(EXIT_FAILURE);
    }
    else if(pid>0){
        // parent process
        if (!isBackgroundJob) {
            //wait for child process to finish its job
            waitpid(pid, nullptr, 0);
        }
        else {
            // current job is background job , so lets add it to our jobs
            std::string command;
            for(auto& token : tokens){
                command += token;
                command += ' ';
            }
            if(!command.empty()) command.pop_back();

            Job newJob = addJob(pid,command);
            cout<<'['<<newJob.id<<"] "<<newJob.pid<<endl;
        }
    }
    else{
        perror("fork");
    }

    return true;
}