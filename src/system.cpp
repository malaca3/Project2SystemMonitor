#include <unistd.h>
#include <cstddef>
#include <set>
#include <string> 
#include <vector>
#include <algorithm>
 
#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"
 
using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    vector<int> ids = LinuxParser::Pids(); 
    vector<Process> exs = cleanProcesses(ids);

    for(auto p : exs){
      processes_.push_back(p);
      processes_.back().CalcCpuUtilization();
   }

    for(auto i : ids){
        processes_.push_back(Process(i));
    }

    std::sort(processes_.begin(), processes_.end());

    //processes_.push_back(Process(ids.back()));
    return processes_; 


}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

std::vector<Process> System::cleanProcesses(std::vector<int>& pids){
    vector<Process> res;
    
    for(auto p : processes_){
        if(p.InList(pids))
            res.push_back(p);
    }

    processes_.clear();

    return res;
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }