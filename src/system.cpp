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


// Return the system's CPU
Processor& System::Cpu() { 
    return cpu_; 
}

// Return a container composed of the system's processes
vector<Process>& System::Processes() {
    auto active_pids = LinuxParser::Pids(); // Get all the running ids

<<<<<<< HEAD
// Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    vector<int> pids = LinuxParser::Pids(); 
    for (int p:pids){
        Process process(p);
        processes_.emplace_back(process);
    }
    std::sort(processes_.begin(), processes_.end()); 
    return processes_;
=======
    // update the set of ative processes
//    active_set = std::union(std::set_intersection(active_set, active_pids), active_pids);

    // scan the vector, delete old processes and add the new ones
    if (!b_){
        for (auto a:active_pids)
            processes_.emplace_back(Process(a));
        b_ = true;
    }
    std::sort(processes_.begin(), processes_.end());
    return processes_; 
>>>>>>> play
}

// Return the system's kernel identifier (string)
std::string System::Kernel() { 
    return LinuxParser::Kernel(); 
}

float System::MemoryUtilization() { 
    return LinuxParser::MemoryUtilization(); 
}

// Return the operating system name
std::string System::OperatingSystem() { 
    return LinuxParser::OperatingSystem(); 
}

// Return the number of processes actively running on the system
int System::RunningProcesses() { 
    return LinuxParser::RunningProcesses(); 
}

// Return the total number of processes on the system
int System::TotalProcesses() { 
    return LinuxParser::TotalProcesses(); 
}

// Return the number of seconds since the system started running
long int System::UpTime() { 
    return LinuxParser::UpTime(); 
}