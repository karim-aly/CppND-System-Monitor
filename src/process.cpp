#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// Constructor
Process::Process(int pid) {
  this->pid = pid;
  // update user string
  this->user = LinuxParser::User(pid);
  // update command string
  this->command = LinuxParser::Command(pid);
}

// Return this process's ID
int Process::Pid() const { return pid; }

// Return this process's CPU utilization
float Process::CpuUtilization() const {
  // Read the current CPU Readings
  const std::pair<long, long> current = LinuxParser::CpuUsage(Pid());

  // Get The Total CPU Time and Total Idle Time from The Previous Reading
  auto previous = previous_cpu_readings;

  // Calculate The Delta
  const long delta_cpu_time = current.first - previous.first;
  const long delta_elapsed_time = current.second - previous.second;

  // Set The Previous Readings to be the new current readings
  previous_cpu_readings = current;

  // return the percentage cpu utilization
  return delta_cpu_time * 1.0f / delta_elapsed_time;
}

// Return the command that generated this process
string Process::Command() const { return this->command; }

// Return this process's memory utilization
string Process::Ram() const { return LinuxParser::Ram(Pid()); }

// Return the user (name) that generated this process
string Process::User() const { return this->user; }

// Return the age of this process (in seconds)
long int Process::UpTime() const { return LinuxParser::UpTime(Pid()); }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  return this->CpuUtilization() < a.CpuUtilization();
}