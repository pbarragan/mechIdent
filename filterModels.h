#ifndef FILTER_MODELS_H
#define FILTER_MODELS_H

#include <vector>
#include "stateStruct.h"

namespace filterModels {
  double logProbState(stateStruct sampleState, stateStruct meanState);
  double logProbObs(std::vector<double> obs, stateStruct state);
}
  
#endif //FILTER_MODELS_H
