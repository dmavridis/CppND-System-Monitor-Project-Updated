#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <iostream>
#include <string>
#include <vector>

class Processor {
    public:
    float Utilization();  // See src/processor.cpp

    private:
    std::vector<std::string> cpu; 
    float utilization;
    // current reading from /proc/stat
    unsigned long long int user;
    unsigned long long int nice;
    unsigned long long int system;
    unsigned long long int idle;
    unsigned long long int iowait;
    unsigned long long int irq;
    unsigned long long int softirq;
    unsigned long long int steal;
    unsigned long long int guest;
    unsigned long long int guest_nice;

    // previous reading for delta calculation
    unsigned long long int prev_idleall{0};
    unsigned long long int prev_total{0};
};

#endif