#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <vector>
#include "mechanisms/mechanism.h"

namespace translator {
  Mechanism* createMechanism(int choice);
  std::vector<double> stateTransition(std::vector<double> state, std::vector<double> action);
  std::vector<double> translateStateToO(std::vector<double> state);
  std::vector<double> translateObsToO(std::vector<double> obs);
}
  
#endif //TRANSLATOR_H
