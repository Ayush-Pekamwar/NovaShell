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

void printJobs(bool onlyDoneJobs){
    updateJobs();
    char marker = ' ';
    std::string status;
    for(int i=0; i<jobs.size(); i++){
        if(i == jobs.size()-1) marker = '+';
        else if(i == jobs.size()-2) marker = '-';
        else marker = ' ';

        if(jobs[i].running) status = "Running                 ";
        else status = "Done                    ";

        if(onlyDoneJobs && jobs[i].running) continue;
        std::cout<<'['<<jobs[i].id<<"]"<<marker<<" "<<status<<jobs[i].command<<std::endl;
    }

    removeDoneJobs();
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

void removeDoneJobs(){
    jobs.erase(
        std::remove_if(jobs.begin(), jobs.end(), [](const Job& j) {
            return !j.running;
        }),
        jobs.end()
    );
}

void updateJobs(){
    for(int i=0; i<jobs.size(); i++){
        int status;
        // returns pid of job > 0 if it is done (not running)
        pid_t resultpid = waitpid(jobs[i].pid , &status, WNOHANG);
        if(resultpid > 0){
            // current job is done , mark it done
            jobs[i].running = false;
        }
    }
}