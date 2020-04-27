#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <string>
#include <vector>

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  std::vector<float> util{0,0,0,0,0,0,0,0,0,0};
  std::vector<float> prev_util{0,0,0,0,0,0,0,0,0,0};

};

#endif