#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int i): pid(i){
    CalcCpuUtilization();
}

// TODO: Return this process's ID
int Process::Pid() { return pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    return cpu; 
 }

 void Process::CalcCpuUtilization() { 
    vector<float> times = LinuxParser::CpuUtilization(pid);
    float starttime = LinuxParser::UpTime(pid);
    float uptime = LinuxParser::UpTime();

    total_time = times[0] + times[1] + times[2] + times[3];
    seconds = uptime - (starttime/sysconf(_SC_CLK_TCK));
    
    cpu = (((total_time-prev_total_time)/sysconf(_SC_CLK_TCK))/(seconds-prev_seconds));
    if(seconds-prev_seconds > 5){
        prev_total_time = total_time;
        prev_seconds = seconds;
    }
 }

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid); }

bool Process::InList(std::vector<int>& ids) {
    for(unsigned int i = 0; i < ids.size(); i++){
        if(ids[i] == pid){
            ids.erase(ids.begin()+i);
            return true;
        }
    }
    return false;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { return cpu > a.cpu; }