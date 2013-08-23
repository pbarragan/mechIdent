#ifndef STATE_STRUCT_H
#define STATE_STRUCT_H

#include <vector>

struct stateStruct {
  int model;
  std::vector<double> params;
  std::vector<double> vars;
}
  
#endif //STATE_STRUCT_H
