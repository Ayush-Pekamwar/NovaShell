#include "jobs.h"
#include <iostream>
#include <vector>
#include <string>

static std::vector<Job> jobs;

void printJobs(){
    for(auto& job: jobs){
        std::cout<<'['<<job.id<<"]+  "<<"Running                 "<<job.command<<std::endl;
    }
}

Job addJob(pid_t pid, std::string& command){
    int id = jobs.size()+1;

    Job newJob;
    newJob.id = id;
    newJob.pid = pid;
    newJob.command = command;
    newJob.running = true;

    jobs.push_back(newJob);

    return newJob;
}

void removeJob(pid_t pid){
    auto idx = -1;
    for(int i=0; i<jobs.size(); i++){
        if(jobs[i].pid == pid){
            idx = i;
            break;
        }
    }

    if(idx != -1){
        jobs.erase(jobs.begin()+idx);
    }
}

void updateJobs(){
    int status;
    pid_t pid;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        std::cout << "[" << pid << "] Done\n";
        removeJob(pid);
    }

}