#include <string>
#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization -> DONE
float Processor::Utilization() {
  std::string line;
  std::string key;
  std::string s_user, s_nice, s_system, s_idle, s_iowait, s_irq, s_softirq, s_steal;
  float user, nice, system, idle, iowait, irq, softirq, steal;
  float Idle, NonIdle;
  std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> s_user >> s_nice >> s_system >> s_idle >> s_iowait >> s_irq >> s_softirq >> s_steal) {
        if (key == "cpu") {
          user = std::stof(s_user);
          nice = std::stof(s_nice);
          system = std::stof(s_system);
          idle = std::stof(s_idle);
          iowait = std::stof(s_iowait);
          irq = std::stof(s_irq);
          softirq = std::stof(s_softirq);
          steal = std::stof(s_steal);
          Idle = idle + iowait;
          NonIdle = user + nice + system + irq + softirq + steal;
          return (NonIdle / (Idle+NonIdle));
        }
      }
    }
  }
  return 0.0; 
}