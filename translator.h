#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <vector>
#include "mechanisms/mechanism.h"
#include "stateStruct.h"

namespace translator {
  Mechanism* createMechanism(int choice);
  stateStruct stateTransition(stateStruct& state, std::vector<double>& action);
  std::vector<double> translateStToObs(stateStruct& state);
  std::vector<double> translateStToRbt(stateStruct& state); 
  std::vector<double> translateSensToObs(std::vector<double>& obs);
}
  
#endif //TRANSLATOR_H
