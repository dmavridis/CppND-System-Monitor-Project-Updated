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
using std::stol;

Process::Process(int pid): pid_{pid} {
    CpuUtilization_();
}   

// Return this process's ID
int Process::Pid() { 
    return pid_; 
}

float Process::CpuUtilization() { 
    return cpu_;
}

void Process::CpuUtilization_() {
  active_jiffies = LinuxParser::ActiveJiffies(Pid());
  long total_jiffies = LinuxParser::Jiffies();
  cpu_ = 1.0*(active_jiffies)/total_jiffies;
}

float Process::Compare() const {
    return cpu_;
}

// Return the command that generated this process
string Process::Command() { 
    return LinuxParser::Command(Pid()); 
}

// Return this process's memory utilization
string Process::Ram() { 
    return LinuxParser::Ram(Pid()); 
}

// Return the user (name) that generated this process
string Process::User() { 
    return LinuxParser::User(Pid()); 
}

// Return the age of this process (in seconds)
long int Process::UpTime() { 
    return LinuxParser::UpTime(Pid());
} 

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
    return Compare() > a.Compare();
}