#include <dirent.h>
#include <unistd.h>
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

string LinuxParser::Kernel() {
  string os, version, kernel;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  std::ifstream fstream(kProcDirectory + kMeminfoFilename);
  std::string line, key, value, mem_total, mem_used;
    long memTotal, memFree;
  if (fstream.is_open()) {
    while (getline(fstream, line)) {
      std::istringstream ss(line);
      ss >> key >> value;
      if (key == "MemTotal:") memTotal = std::stol(value);
      if (key == "MemFree:") memFree = std::stol(value);
    }
    
  } else return 0;
  return (memTotal - memFree) / (double)memTotal;
}
// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  std::string s_number;
  std::string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    getline(stream, line);
    std::istringstream sstream(line);
    sstream >> s_number;
  }
  return std::stol(s_number);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  std::ifstream fs(kProcDirectory + kStatFilename);
  string cpu;
  std::vector<string> cpu_values;
  if (fs.is_open()) {
    string line;
    getline(fs, line);
    std::istringstream iss(line);

    if (iss >> cpu) {
      for (string val; iss >> val;) {
        cpu_values.push_back(val);
      }
    }
  }
  return cpu_values;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::string line, key;
  int value;
  std::ifstream fileStream(kProcDirectory + kStatFilename);
  if (fileStream.is_open()) {
    while (getline(fileStream, line)) {
      std::istringstream lineStream(line);
      lineStream >> key >> value;
      if (key == "processes") return value;
    }
  }

  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::string line, key;
  int value;
  std::ifstream fileStream(kProcDirectory + kStatFilename);
  if (fileStream.is_open()) {
    while (getline(fileStream, line)) {
      std::istringstream lineStream(line);
      lineStream >> key >> value;
      if (key == "procs_running") return value;
    }
  }

  return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  std::ifstream fs(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  string cmd{};

  if (fs.is_open()) {
    fs >> cmd;
  }

  return cmd;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  std::ifstream fs(kProcDirectory + std::to_string(pid) + kStatusFilename);
  long value = 0;
  if (fs.is_open()) {
    std::string line, key;
    while (getline(fs, line)) {
      std::istringstream lineStream(line);
      lineStream >> key >> value;
      if (key == "VmSize:") break;
    }
  }

  return std::to_string(value / 1024);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  std::ifstream fs(kProcDirectory + std::to_string(pid) + kStatusFilename);
   std::string line, key, value;
  if (fs.is_open()) {
    while (getline(fs, line)) {
      std::istringstream lineStream(line);
      lineStream >> key >> value;
      if (key == "Uid:") break;
    }
  }
  else return {};
  return value;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string uid = LinuxParser::Uid(pid);
  std::ifstream fs("/etc/passwd");
  string name, pwd, id;
  if (fs.is_open()) {
    string line;
    while (getline(fs, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream iss(line);
      iss >> name >> pwd >> id;
      if (uid == id) return name;
    }
  }
  return name;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  std::ifstream fs(kProcDirectory + std::to_string(pid) + kStatFilename);
  std::vector<std::string> vals;
  if (fs.is_open()) {
    string line;
    getline(fs, line);
    std::istringstream iss(line);
    string s;
    while (iss >> s) {
      vals.push_back(s);
    }
  }
  else return 0;
  return std::stol(vals[21]) / sysconf(_SC_CLK_TCK);
}