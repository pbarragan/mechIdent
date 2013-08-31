#ifndef BAYES_FILTER_H
#define BAYES_FILTER_H

#include <vector>
#include "stateStruct.h"

class BayesFilter {
  
 public:

  //variables
  std::vector<stateStruct> stateList_;
  std::vector<double> logProbList_;

  //functions
  BayesFilter();
  void transitionUpdateLog(std::vector<double> action);
  void transitionUpdateLog(std::vector<double>& logProbList, std::vector<double> action);
  void observationUpdateLog(std::vector<double> obs);
  void observationUpdateLog(std::vector<double>& logProbList, std::vector<double> obs);
  void printLogProbList();
  void printStateList();

}; 
#endif //BAYES_FILTER_H
