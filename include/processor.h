#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include <string>

class Processor {
 public:
  float Utilization();

 private:
  std::vector<string> previous_cpu_readings;
};

#endif