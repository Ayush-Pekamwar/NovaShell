#include "jobs.h"
#include <iostream>
#include <vector>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


static std::vector<Job> jobs;

int getJobid(pid_t pid){
    int id = -1;
    for(int i=0; i<jobs.size(); i++){
        if(jobs[i].pid == pid){
            id = jobs[i].id;
            break;
        }
    }
    return id;
}

int findJobWithPid(pid_t pid){
    // returns job index in vector of jobs
    int id = -1;
    for(int i=0; i<jobs.size(); i++){
        if(jobs[i].pid == pid){
            id = i;
            break;
        }
    }
    return id;
}

void printJobs(){
    updateJobs();
    char marker = ' ';
    for(int i=0; i<jobs.size(); i++){
        if(i == jobs.size()-1) marker = '+';
        else if(i == jobs.size()-2) marker = '-';
        else marker = ' ';

        std::cout<<'['<<jobs[i].id<<"]"<<marker<<"  Running                 "<<jobs[i].command<<std::endl;
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
        int idx = findJobWithPid(pid);
        if(idx == -1) continue;
        std::cout << "[" << jobs[idx].id << "]+"<<"Done                    "<<jobs[idx].command<<std::endl;
        removeJob(pid);
    }

}