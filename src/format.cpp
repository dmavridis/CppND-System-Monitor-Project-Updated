#include <string>

#include "format.h"

using std::string;
using std::to_string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS

string Format::ElapsedTime(long seconds) { 
    int hh = int(seconds/3600);
    int mm = int((seconds - 3600*hh)/60);
    int ss = seconds - 3600*hh - 60*mm;
 
    return  to_string(hh) + ":" + to_string(mm) + ":" + to_string(ss);
}