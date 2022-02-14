#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid, string user, float cpuutilization, string ram, long int uptime, string command) 
    : pid_(pid), user_(user), cpuutilization_(cpuutilization), ram_(ram), uptime_(uptime), command_(command) {}  // constructor (added)

// TODO: Return this process's ID -> DONE
int Process::Pid() const { return pid_; }

// TODO: Return this process's CPU utilization -> DONE
float Process::CpuUtilization() const { return cpuutilization_; }

// TODO: Return the command that generated this process -> DONE
string Process::Command() const { return command_; }

// TODO: Return this process's memory utilization -> DONE
string Process::Ram() const { return ram_; }

// TODO: Return the user (name) that generated this process -> DONE
string Process::User() const { return user_; }

// TODO: Return the age of this process (in seconds) -> DONE
long int Process::UpTime() const { return uptime_; }

// TODO: Overload the "less than" comparison operator for Process objects -> DONE
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
    if (cpuutilization_ < a.cpuutilization_)
        return false;
    else
        return true;
}