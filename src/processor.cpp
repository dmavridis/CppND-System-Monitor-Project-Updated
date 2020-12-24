#include <vector>

#include "processor.h"
#include "linux_parser.h"

using std::vector;
using std::string;
using std::stoi;

// Return the aggregate CPU utilization
// See https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
float Processor::Utilization() { 
    jiffies_ = LinuxParser::Jiffies();
    active_jiffies_ = LinuxParser::ActiveJiffies();
    
    utilization = 1.0*(active_jiffies_ - prev_active_jiffies_)/(jiffies_ - prev_jiffies_);

    prev_jiffies_ = jiffies_;
    prev_active_jiffies_ = active_jiffies_;
    return utilization;
}