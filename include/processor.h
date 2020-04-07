#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <string>
#include <vector>

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  std::vector<long> cpu_values_{};
  void getCPUvalues();
  float calStableUtil();
  long totalTime_ = 0;
  long activeTime_ = 0;
};

#endif