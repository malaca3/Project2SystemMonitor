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

Processor& System::Cpu() { return cpu_; }

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

    return processes_; 


}

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

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }