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


<<<<<<< HEAD
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(0.0, 1.0);
    float cpu_usage = dis(gen);
    return cpu_usage; 
=======
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
>>>>>>> play
}

// Return the command that generated this process
string Process::Command() { 
<<<<<<< HEAD
    return LinuxParser::Command(pid_); 
=======
    return LinuxParser::Command(Pid()); 
>>>>>>> play
}

// Return this process's memory utilization
string Process::Ram() { 
<<<<<<< HEAD
    return LinuxParser::Ram(pid_); 
=======
    return LinuxParser::Ram(Pid()); 
>>>>>>> play
}

// Return the user (name) that generated this process
string Process::User() { 
<<<<<<< HEAD
    return LinuxParser::User(pid_); 
=======
    return LinuxParser::User(Pid()); 
>>>>>>> play
}

// Return the age of this process (in seconds)
long int Process::UpTime() { 
<<<<<<< HEAD
    return LinuxParser::UpTime(pid_);
} 

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process &a)  { 
    return CpuUtilization() < a.CpuUtilization(); 
=======
    return LinuxParser::UpTime(Pid());
} 

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
    return Compare() < a.Compare(); 
>>>>>>> play
}