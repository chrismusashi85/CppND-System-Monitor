#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid, string user, string command, float cpuutilization, string ram, long int uptime) 
    : pid_(pid), user_(user), command_(command), cpuutilization_(cpuutilization), ram_(ram), uptime_(uptime) {}  // constructor (added)

// Return this process's ID -> DONE
int Process::Pid() const { return pid_; }

// Return this process's CPU utilization -> DONE
float Process::CpuUtilization() const { return cpuutilization_; }

// Return the command that generated this process -> DONE
string Process::Command() const { return command_; }

// Return this process's memory utilization -> DONE
string Process::Ram() const { return ram_; }

// Return the user (name) that generated this process -> DONE
string Process::User() const { return user_; }

// Return the age of this process (in seconds) -> DONE
long int Process::UpTime() const { return LinuxParser::UpTime() - uptime_; }

// Overload the "less than" comparison operator for Process objects -> DONE
bool Process::operator<(const Process& a) const {
    return (a.cpuutilization_ < this->cpuutilization_);
}