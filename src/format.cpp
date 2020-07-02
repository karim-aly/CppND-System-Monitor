#include <string>
#include <iomanip>
#include <sstream>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  // get hours count
  int h = seconds / (60 * 60);
  seconds -= h * (60 * 60);

  // get minutes count
  int m = seconds / 60;
  seconds -= m * 60;

  // format the string using a string stream
  std::ostringstream ss;
  ss << std::setw(2) << std::setfill('0') << h;
  ss << std::setw(1) << ':';
  ss << std::setw(2) << m;
  ss << std::setw(1) << ':';
  ss << std::setw(2)  << seconds;

  return ss.str();
}