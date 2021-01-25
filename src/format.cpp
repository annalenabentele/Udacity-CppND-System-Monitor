#include <string>
#include <iomanip>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    int hours = seconds / (60 * 60);
    seconds -= hours * (60 * 60);

    int mins = seconds / (60);
    seconds -= mins * (60);

    std::ostringstream os;
    os << std::setw(2) << std::setfill('0') << hours << ":" 
       << std::setw(2) << std::setfill('0') << mins << ":"
       << std::setw(2) << std::setfill('0') << seconds;
    
    return os.str();
}