#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <iostream>
#include <string>
#include <vector>

class Processor {
    public:
    float Utilization();  // See src/processor.cpp

    private:
    float utilization;
    unsigned long long int active_jiffies_{0};
    unsigned long long int jiffies_{0};
    unsigned long long int prev_active_jiffies_{0};
    unsigned long long int prev_jiffies_{0};

};

#endif