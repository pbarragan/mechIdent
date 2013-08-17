#ifndef FILTER_MODELS_H
#define FILTER_MODELS_H

#include <vector>

namespace filterModels {
  double logProbState(std::vector<double> sampleState, std::vector<double> meanState);
  double logProbObs(std::vector<double> obs, std::vector<double> state);

}
  
#endif //FILTER_MODELS_H
