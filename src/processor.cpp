#include <string>
#include <vector>
#include "linux_parser.h"
#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
float prevIdleT = 0, prevNonIdle = 0, prevTotal = 0;
float idleT = 0, nonIdle = 0, totald = 0, idled = 0, CPU_Percentage = 0, total = 0;

util = LinuxParser::CpuUtilization();

idleT = util[3] + util[4];
prevIdleT = prev_util[3] + prev_util[4];

prevNonIdle = prev_util[0] + prev_util[1] + prev_util[2] + prev_util[5] + prev_util[6] + prev_util[7];
nonIdle = util[0] + util[1] + util[2] + util[5] + util[6] + util[7];

prevTotal = prevIdleT + prevNonIdle;
total = idleT + nonIdle;

totald = total - prevTotal;
idled = idleT - prevIdleT;

CPU_Percentage = (totald - idled)/totald;

prev_util = util;

return CPU_Percentage; 
}