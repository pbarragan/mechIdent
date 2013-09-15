#ifndef ACTION_SELECTION_H
#define ACTION_SELECTION_H

#include <vector>
#include "stateStruct.h"
#include "sasUtils.h"
#include "bayesFilter.h"

namespace actionSelection {
  
  void chooseActionSimple(std::vector< std::vector<double> >& actionList,int step,std::vector<double>& action);
  void chooseActionRandom(std::vector< std::vector<double> >& actionList,std::vector<double>& action);
  // overloaded
  void chooseActionLog(BayesFilter& filter,std::vector< std::vector<double> >& actionList,std::vector<double>& action,std::vector<stateStruct>& modelParamPairs);
  void chooseActionLog(BayesFilter& filter,std::vector< std::vector<double> >& actionList,std::vector<double>& action,std::vector<stateStruct>& modelParamPairs,sasUtils::mapPairSVS& sasList);

  std::vector<double> getNoisyObs(stateStruct& state);
  std::vector<double> createCDF(std::vector<double>& probList);
  stateStruct getSampleState(std::vector<double>& CDF, std::vector<stateStruct>& states);
  double calcEntropy(std::vector<double> probs);
  double randomDouble();

}
  
#endif //ACTION_SELECTION_H
