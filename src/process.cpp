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


// Play area
#include <random>


// Return this process's ID
int Process::Pid() { 
    return pid_; 
}

// Return this process's CPU utilization
float Process::CpuUtilization() { 
    // std::random_device rd;  //Will be used to obtain a seed for the random number engine
    // std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    // std::uniform_real_distribution<> dis(0.0, 1.0);
    // cpu_ = dis(gen);
    cpu_ = LinuxParser::ActiveJiffies(Pid()); //LinuxParser::UpTime(Pid());
    return cpu_;     
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
    return Compare() < a.Compare(); 
}