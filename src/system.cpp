#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
using std::to_string;

/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// ADD: compare processes with cpuutilization
bool Compare(const Process& a, const Process& b) {
    return (a < b);
}

// Return a container composed of the system's processes -> DONE
vector<Process>& System::Processes() {
    processes_.clear();
    for (int pid : LinuxParser::Pids()) {
        std::string user = LinuxParser::User(pid); // username of process
        float cpuutilization = LinuxParser::CpuUtilization(pid); // cpu utilization of process
        std::string ram = LinuxParser::Ram(pid); // memory utilization of process
        long int uptime = LinuxParser::UpTime(pid); // uptime of process
        std::string command = LinuxParser::Command(pid); // command which invokes process
        processes_.emplace_back(Process(pid, user, command, cpuutilization, ram, uptime));
    }
    //if (processes_.size() > 0)
    std::sort(processes_.begin(), processes_.end(), Compare);
    return processes_;
}

// Return the system's kernel identifier (string) -> DONE
std::string System::Kernel() { return string(LinuxParser::Kernel()); }

// Return the system's memory utilization -> DONE
float System::MemoryUtilization() { return (LinuxParser::MemoryUtilization()); }

// Return the operating system name -> DONE
std::string System::OperatingSystem() { return string(LinuxParser::OperatingSystem()); }

// Return the number of processes actively running on the system -> DONE
int System::RunningProcesses() { return (LinuxParser::RunningProcesses()); }

// Return the total number of processes on the system -> DONE
int System::TotalProcesses() { return (LinuxParser::TotalProcesses()); }

// Return the number of seconds since the system started running -> DONE
long int System::UpTime() { return LinuxParser::UpTime(); }
