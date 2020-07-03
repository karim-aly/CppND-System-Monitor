#include<vector>
#include<string>

#include "processor.h"
#include "linux_parser.h"

using std::vector;
using std::string;
using std::stol;


// Helper Function to Calculate The Total Utilization and Idle Time of CPU
std::pair<long, long> CalculateAggregate(const vector<string> &cpu_readings) {
  // check first that the cpu_readings are indeed 10 values
  if (cpu_readings.size() == 10) {
    return {0,0};
  }

  // unpack the vector and convert each reading to long
  long user = stol(cpu_readings[0]);
  long nice = stol(cpu_readings[1]);
  long system = stol(cpu_readings[2]);
  long idle = stol(cpu_readings[3]);
  long iowait = stol(cpu_readings[4]);
  long irq = stol(cpu_readings[5]);
  long softirq = stol(cpu_readings[6]);
  long steal = stol(cpu_readings[7]);
  long guest = stol(cpu_readings[8]);
  long guest_nice = stol(cpu_readings[9]);

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

  // Get The Total CPU Time and Total Idle Time for Current Reading
  auto previous = CalculateAggregate(previous_cpu_readings);

  // Calculate The Delta
  const long delta_total = current.first - previous.first;
  const long delta_idle = current.second - previous.second;

  return (delta_total - delta_idle) * 1.0f / delta_total;
}