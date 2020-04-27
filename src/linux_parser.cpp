#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream> 
#include <math.h>
#include <unistd.h>
 
#include "linux_parser.h"
#include "format.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;



// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() { 
  string memory;
  string line;
  float free = 0, total = 0;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> memory >> total;

    std::getline(stream, line);
    std::istringstream linestream2(line);
    linestream2 >> memory >> free;
  }
  return (total - free)/total;
}

long LinuxParser::UpTime() { 
  string line;
  long time = 0;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >>  time;
  }
  return time;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { return pid; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

vector<float> LinuxParser::CpuUtilization() { 
  string line, cpu;
  vector<float> res(10);
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> res[0] >> res[1] >> res[2] >> res[3] >> res[4] >> res[5] >> res[6] >> res[7] >> res[8] >> res[9];
  }
  return res; 
}

int LinuxParser::TotalProcesses() { 
  string processes, line;
  int total;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> processes >> total;
      if(processes == "processes")
        break;
      }
  }
  return total;
}

int LinuxParser::RunningProcesses() { 
  string processes, line;
  int total;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> processes >> total;
      if(processes == "procs_running")
        break;
      }
  }
  return total;
 }

string LinuxParser::Command(int pid) { 
  string cmd, line;
  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> cmd;
  }
  return cmd;
 }

string LinuxParser::Ram(int pid) { 
  string info, line, tot;
  
  float total;
  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> info >> total;
      if(info == "VmSize:"){
        tot = Format::to_string_with_precision(total*0.001, 2);
        break;
      }
      }
  }
  return tot;
}

string LinuxParser::Uid(int pid) { 
  string info, line, uid;
  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> info >> uid;
      if(info == "Uid:")
        break;
      }
  }
  return uid; 
}

string LinuxParser::User(int pid) {  
string line, uid = Uid(pid), x, user, aux_uid;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while(std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> x >> aux_uid;
      if(aux_uid == uid)
        break;
      }
  }
  return user; 
}

long LinuxParser::UpTime(int pid) { 
  string line, time_str;
  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid)  + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
      std::istringstream linestream(line);
      for(int i = 0; i < 22; i++)
        linestream >> time_str;
  }
  return LinuxParser::UpTime() - stol(time_str)/sysconf(_SC_CLK_TCK);
}

std::vector<float> LinuxParser::CpuUtilization(int pid){
  string line, utime, stime, cutime, cstime;
  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid)  + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
      std::istringstream linestream(line);
      for(int i = 0; i < 14; i++)
        linestream >> utime;
      linestream >> stime >> cutime >> cstime;
  }
  vector<float> res{stof(utime), stof(stime), stof(cutime), stof(cstime)};
  
  return res;
}