#include <string>
#include <sstream>
#include <iomanip>

#include "format.h"

using std::string;

// TODO: Complete this helper function -> DONE
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    long hour;
    long minute;
    long second;
    std::ostringstream oss;

    hour = seconds/3600;
    minute = (seconds - hour*3600)/60;
    second = seconds % 60;
    oss << std::setw(2) << std::setfill('0') << hour << ":" << std::setw(2) << std::setfill('0') << minute << ":" << std::setw(2) << std::setfill('0') << second;

    return oss.str();
}