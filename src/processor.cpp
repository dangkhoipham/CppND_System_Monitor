#include "processor.h"
#include "linux_parser.h"
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  getCPUvalues();
  return calStableUtil();
}

void Processor::getCPUvalues() {
  std::vector<std::string> cpu_vals = LinuxParser::CpuUtilization();
  cpu_values_ = std::vector<long>{};
  for (auto item : cpu_vals) {
    cpu_values_.push_back(std::stol(item));
  }
}

float Processor::calStableUtil() {
  long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  user = cpu_values_[0];
  nice = cpu_values_[1];
  system = cpu_values_[2];
  idle = cpu_values_[3];
  iowait = cpu_values_[4];
  irq = cpu_values_[5];
  softirq = cpu_values_[6];
  steal = cpu_values_[7];
  guest = cpu_values_[8];
  guest_nice = cpu_values_[9];

  user = user - guest;
  nice = nice - guest_nice;

  long idlealltime = idle + iowait;
  long systemalltime = system + irq + softirq;
  long virtalltime = guest + guest_nice;
  totalTime_ = user + nice + systemalltime + idlealltime + steal + virtalltime;
  activeTime_ = user + nice + system + irq + softirq + steal;
  return activeTime_ / (double)totalTime_;
}