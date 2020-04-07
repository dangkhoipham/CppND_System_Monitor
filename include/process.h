#ifndef PROCESS_H
#define PROCESS_H

#include <cstring>
#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                   // TODO: See src/process.cpp
  std::string User();          // TODO: See src/process.cpp
  std::string Command();       // TODO: See src/process.cpp
  float CpuUtilization() const;      // TODO: See src/process.cpp
  std::string Ram();           // TODO: See src/process.cpp
  long int UpTime();           // TODO: See src/process.cpp
  bool operator<(const Process& a) const;  // TODO: See src/process.cpp
  Process(int id) : id_(id) {}
  int getID() const {
    return id_;
  }  // TODO: Declare any necessary private members
  int id_ = 0;


 private:
};

#endif