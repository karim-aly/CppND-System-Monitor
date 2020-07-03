#include<vector>
#include<string>

#include "processor.h"
#include "linux_parser.h"

using std::vector;
using std::string;
using std::stol;

using LinuxParser::CPUStates;


// Helper Function to Calculate The Total Utilization and Idle Time of CPU
std::pair<long, long> CalculateAggregate(const vector<string> &cpu_readings) {
  // check first that the cpu_readings are indeed 10 values
  if (cpu_readings.size() != 10) {
    return {0,0};
  }

  // unpack the vector and convert each reading to long
  long user       =   stol(cpu_readings[CPUStates::kUser_]);
  long nice       =   stol(cpu_readings[CPUStates::kNice_]);
  long system     =   stol(cpu_readings[CPUStates::kSystem_]);
  long idle       =   stol(cpu_readings[CPUStates::kIdle_]);
  long iowait     =   stol(cpu_readings[CPUStates::kIOwait_]);
  long irq        =   stol(cpu_readings[CPUStates::kIRQ_]);
  long softirq    =   stol(cpu_readings[CPUStates::kSoftIRQ_]);
  long steal      =   stol(cpu_readings[CPUStates::kSteal_]);
  long guest      =   stol(cpu_readings[CPUStates::kGuest_]);
  long guest_nice =   stol(cpu_readings[CPUStates::kGuestNice_]);

  // calculate the cpu idle time
  long Idle = idle + iowait;

  // calculate the cpu non-idle time
  long NonIdle = user + nice + system + irq + softirq + steal;

  // cpu total time
  long Total = Idle + NonIdle;

  return {Total, Idle};
}

// Return the aggregate CPU utilization
float Processor::Utilization() {
  // Read the current CPU Readings
  const vector<string> &cpu_readings = LinuxParser::CpuUtilization();

  // Get The Total CPU Time and Total Idle Time for Current Reading
  auto current = CalculateAggregate(cpu_readings);

  // Get The Total CPU Time and Total Idle Time from The Previous Reading
  auto previous = previous_cpu_readings;

  // Calculate The Delta
  const long delta_total = current.first - previous.first;
  const long delta_idle = current.second - previous.second;
  const long delta_active = delta_total - delta_idle;

  // Set The Previous Readings to be the new current readings
  previous_cpu_readings = current;

  // return the percentage cpu utilization
  return delta_active * 1.0f / delta_total;
}