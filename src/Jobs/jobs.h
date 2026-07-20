#include <string>
#include <unistd.h>

struct Job {
    int id;
    pid_t pid;
    std::string command;
    bool running;
};

Job addJob(pid_t pid, std::string& command);
void removeDoneJobs();
void printJobs();
void updateJobs();
int getJobid(pid_t pid);
int findJobWithPid(pid_t pid);