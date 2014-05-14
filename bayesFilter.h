#ifndef BAYES_FILTER_H
#define BAYES_FILTER_H

#include <vector>
#include "stateStruct.h"
#include "sasUtils.h"

class BayesFilter {
  
 public:

  //variables
  std::vector<stateStruct> stateList_;
  std::vector<double> logProbList_;

  // these are just used to understand the filter
  std::vector<double> logProbList_T_;
  std::vector<double> logProbList_O_;


  //functions
  BayesFilter();

  // MEGA overloaded
  void transitionUpdateLog(std::vector<double> action, sasUtils::mapPairSVS& sasList);
  void transitionUpdateLog(std::vector<double>& logProbList, std::vector<double> action, sasUtils::mapPairSVS& sasList);
  void transitionUpdateLog(std::vector<double> action);
  void transitionUpdateLog(std::vector<double>& logProbList, std::vector<double> action);

  // overloaded
  void observationUpdateLog(std::vector<double> obs);
  void observationUpdateLog(std::vector<double>& logProbList, std::vector<double> obs);

  void printLogProbList();
  void printStateList();
  void printStatesAndProbs();
}; 
#endif //BAYES_FILTER_H
