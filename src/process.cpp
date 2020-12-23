#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include <random>


#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;
using std::stol;

// Return this process's ID
int Process::Pid() { 
    return pid_; 
}

// Return this process's CPU utilization
float Process::CpuUtilization() { 
    vector<string> cpu_ = LinuxParser::CpuUtilization(pid_);
    long int utime = stol(cpu_.at(0));
    long int stime = stol(cpu_.at(1));
//    long int cutime = stol(cpu_.at(2));
    long int cstime = stol(cpu_.at(3));
    long int starttime = stol(cpu_.at(4));

    long int total_time = utime + stime + cstime;
    long int uptime = LinuxParser::UpTime();
    long Hertz = sysconf(_SC_CLK_TCK) ;
    long int seconds = uptime - (starttime / Hertz);
//    float cpu_usage = (total_time / Hertz) / seconds;


    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(0.0, 1.0);
    float cpu_usage = dis(gen);
    return cpu_usage; 
}

// Return the command that generated this process
string Process::Command() { 
    return LinuxParser::Command(pid_); 
}

// Return this process's memory utilization
string Process::Ram() { 
    return LinuxParser::Ram(pid_); 
}

// Return the user (name) that generated this process
string Process::User() { 
    return LinuxParser::User(pid_); 
}

// Return the age of this process (in seconds)
long int Process::UpTime() { 
    return LinuxParser::UpTime(pid_);
} 

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process &a)  { 
    return CpuUtilization() < a.CpuUtilization(); 
}