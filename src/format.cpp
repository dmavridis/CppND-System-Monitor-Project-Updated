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
 
    string hh_str = to_string(hh);
    string mm_str = to_string(mm);
    string ss_str = to_string(ss);

    // Zero padding if not double digit
    mm_str.insert(mm_str.begin(), 2 - mm_str.length(), '0');
    ss_str.insert(ss_str.begin(), 2 - ss_str.length(), '0');
    // Hours can have more than two digits
    if (hh_str.length()<2)
        hh_str.insert(hh_str.begin(), 2 - hh_str.length(), '0');

    return  hh_str + ":" + mm_str + ":" + ss_str;
}