#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include <string>

class Processor {
 public:
  float Utilization();

 private:
  std::pair<long, long> previous_cpu_readings{0,0};
};

#endif