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

int Process::Pid() { return pid; }

float Process::CpuUtilization() { 
    return cpu; 
 }

 void Process::CalcCpuUtilization() { 
    vector<float> times = LinuxParser::CpuUtilization(pid);
    float uptime = LinuxParser::UpTime(pid);

    total_time = times[0] + times[1] + times[2] + times[3];
    seconds = uptime;
    
    cpu = (((total_time-prev_total_time)/sysconf(_SC_CLK_TCK))/(seconds-prev_seconds));
    if(seconds-prev_seconds > 5){
        prev_total_time = total_time;
        prev_seconds = seconds;
    }
 }

string Process::Command() { return LinuxParser::Command(pid); }

string Process::Ram() { return LinuxParser::Ram(pid); }

string Process::User() { return LinuxParser::User(pid); }

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

bool Process::operator<(Process const& a) const { return cpu > a.cpu; }