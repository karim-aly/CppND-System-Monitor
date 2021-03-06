#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int);
  int Pid() const;
  std::string User() const;
  std::string Command() const;
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram() const;
  long int UpTime() const;
  bool operator<(Process const& a) const;
  bool operator>(Process const& a) const;

 private:
  int pid;
  std::string user;
  std::string command;
  float cpuUtilizationSaved = 0.0f;
  std::pair<long, long> previous_cpu_readings{0,0};
};

#endif