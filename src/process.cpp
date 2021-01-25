#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid) { cpu_ = Process::CpuUtilization(); }

int Process::Pid() { return pid_; }

float Process::Cpu() { return cpu_; }

float Process::CpuUtilization() const { 
    vector<string> values = LinuxParser::CpuUtilization(pid_);
    float totaltime = 0.0;
    float utime, stime, cutime, cstime, starttime = 0.0;

    utime = std::stoi(values[13]);
    stime = std::stoi(values[14]);
    cutime = std::stoi(values[15]);
    cstime = std::stoi(values[16]);
    starttime = std::stoi(values[21]);

    totaltime = utime + stime + cutime + cstime;
    float seconds = LinuxParser::UpTime() - (starttime / sysconf(_SC_CLK_TCK));
    
    return (totaltime / sysconf(_SC_CLK_TCK)) / seconds; 
}

string Process::Command() { return LinuxParser::Command(pid_); }

string Process::Ram() { return LinuxParser::Ram(pid_); }

string Process::User() { return LinuxParser::User(pid_);}

long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

bool Process::operator<(Process const& a) const { 
    return CpuUtilization() < a.CpuUtilization();
}