#include <vector>

#include "processor.h"
#include "linux_parser.h"

using std::vector;
using std::string;
using std::stoi;

// Return the aggregate CPU utilization
// See https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
float Processor::Utilization() { 
    
    
    cpu = LinuxParser::CpuUtilization();
    user = stoi(cpu.at(0));
    nice = stoi(cpu.at(1));
    system = stoi(cpu.at(2));
    idle = stoi(cpu.at(3));
    iowait = stoi(cpu.at(4));
    irq = stoi(cpu.at(5));
    softirq = stoi(cpu.at(6));
    steal = stoi(cpu.at(7));
    guest = stoi(cpu.at(8));
    guest_nice = stoi(cpu.at(9));

    user = user - guest;   
    nice = nice - guest_nice; 
    unsigned long long int idleall = idle + iowait;  // ioWait is added in the idleTime
    unsigned long long int systemall = system + irq + softirq;
    unsigned long long int virtall = guest + guest_nice;
    unsigned long long int total = user + nice + systemall + idleall + steal + virtall;

    unsigned long long int total_d = total - prev_total;
    unsigned long long int idle_d = idleall - prev_idleall;
   
    utilization = 1.0*(total_d-idle_d)/total_d;

    prev_idleall = idleall;
    prev_total = total;

    return utilization;
}