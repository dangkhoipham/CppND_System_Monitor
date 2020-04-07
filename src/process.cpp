#include <unistd.h>
#include <cctype>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;
using namespace LinuxParser;

// TODO: Return this process's ID
int Process::Pid() { return this->id_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const {
  std::ifstream sf(kProcDirectory + std::to_string(id_) + kStatFilename);
  std::vector<string> vals;
  if (sf.is_open()) {
    string line;
    getline(sf, line);
    std::istringstream iss(line);

    string val;
    while (iss >> val) {
      vals.push_back(val);
    }
  } else
    return 0;
  long utime, stime, cutime, cstime, starttime;
  utime = std::stol(vals[13]) / sysconf(_SC_CLK_TCK);
  stime = std::stol(vals[14]) / sysconf(_SC_CLK_TCK);
  cutime = std::stol(vals[15]) / sysconf(_SC_CLK_TCK);
  cstime = std::stol(vals[16]) / sysconf(_SC_CLK_TCK);
  starttime = std::stol(vals[21]) / sysconf(_SC_CLK_TCK);

  long total_time = utime + stime + cutime + cstime;
  long sys_uptime = LinuxParser::UpTime();
  long duration = sys_uptime - starttime;
  
  return total_time / (double)duration;
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(id_); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(id_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(id_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(id_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(const Process& a) const {
  return CpuUtilization() < a.CpuUtilization();
}

