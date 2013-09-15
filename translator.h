#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <vector>
#include "mechanisms/mechanism.h"
#include "stateStruct.h"
#include "sasUtils.h"

namespace translator {
  Mechanism* createMechanism(int choice);
  // overloaded
  stateStruct stateTransition(stateStruct& state, std::vector<double>& action);
  stateStruct stateTransition(stateStruct& state, std::vector<double>& action, sasUtils::mapPairSVS& sasList);

  std::vector<double> translateStToObs(stateStruct& state);
  std::vector<double> translateStToRbt(stateStruct& state); 
  std::vector<double> translateSensToObs(std::vector<double>& obs);

  bool isStateValid(stateStruct& state,std::vector< std::vector<double> >& workspace);
}
  
#endif //TRANSLATOR_H
