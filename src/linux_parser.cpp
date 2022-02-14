#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization -> DONE
float LinuxParser::MemoryUtilization() { 
  string line;
  string key;
  string value;
  float MemTotal = 0;
  float MemFree = 0;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          MemTotal = stof(value);
        }
        if (key == "MemFree:") {
          MemFree = stof(value);
          if (MemTotal != 0)
            return (MemTotal - MemFree)/MemTotal;
        }
      }
    }
  }
  return 0.;
}

// TODO: Read and return the system uptime -> DONE
long LinuxParser::UpTime() {
  long uptime;
  string uptime_str, idletime_str;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime_str >> idletime_str;
    uptime = stol(uptime_str);
  }
  return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
//long LinuxParser::ActiveJiffies(int pid) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization -> DONE
float LinuxParser::CpuUtilization(int pid) {
  string dummy, utime_str, stime_str, cutime_str, cstime_str, starttime_str;
  float utime, stime, starttime, total_time, seconds;
  string line;
  long herts = sysconf(_SC_CLK_TCK);

  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 0; i<13; i++)
      linestream >> dummy;
      
    linestream >> utime_str; // #14
    utime = std::stof(utime_str);
    linestream >> stime_str; // #15
    stime = std::stof(stime_str);
    linestream >> cutime_str; // #16
    linestream >> cstime_str; // #17
    linestream >> starttime_str; // #22
    starttime = std::stof(starttime_str);
    total_time = utime + stime;
    seconds = (float)UpTime() - (starttime / herts);
    return ((total_time / herts) / seconds);
  }
  return (0.0);
}

// TODO: Read and return the total number of processes -> DONE
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return (stoi(value));
        }
      }
    }
  }
  return 0;  
}

// TODO: Read and return the number of running processes -> DONE
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return (stoi(value));
        }
      }
    }
  }
  return 0;  
}

// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line, command_str = "";

  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> command_str;
  }
  return command_str;
}

// TODO: Read and return the memory used by a process -> DONE
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          return (std::to_string(std::stoi(value)/1024));
        }
      }
    }
  }
  return ("");
}

// TODO: Read and return the user ID associated with a process -> DONE
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return(value);
        }
      }
    }
  }
  return("");
}

// TODO: Read and return the user associated with a process -> DONE
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string uid = Uid(pid);
  string passwd;
  string line;
  string key;
  string value;
  std::ifstream filestream(LinuxParser::kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> value >> passwd >> key) {
        if (key == uid) {
          return value;
        }
      }
    }
  }
  return ("");
}

// TODO: Read and return the uptime of a process -> DONE
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line, dummy, uptime_str = "0";
  long herts = sysconf(_SC_CLK_TCK);

  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 0; i<21; i++)
      linestream >> dummy;

    linestream >> uptime_str;

  }
  return (std::stol(uptime_str)/herts);
}
