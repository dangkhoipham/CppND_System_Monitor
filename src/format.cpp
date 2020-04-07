#include "format.h"
#include <iomanip>
#include <sstream>
#include <string>

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  string time;
  int h = seconds / 3600;
  int m = (seconds - h * 3600) / 60;
  int s = seconds % 60;
  string display;
  std::stringstream ss;
  ss << std::setw(2) << std::setfill('0');
  ss << h << ":" << std::setw(2) << std::setfill('0') << m << ":"
     << std::setw(2) << std::setfill('0') << s;

  return string(ss.str());
}