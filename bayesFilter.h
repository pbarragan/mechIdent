#ifndef BAYES_FILTER_H
#define BAYES_FILTER_H

#include <vector>

class bayesFilter {
  
 public:

  //variables
  std::vector< std::vector<double> > stateList_;
  std::vector<double> logProbList_;

  //functions
  void filterSetup(std::vector<double>& stateList, std::vector<double>& logProbList);
  void transitionUpdateLog(std::vector<double> action);
  void transitionUpdateLog(std::vector<double>& logProbList, std::vector<double> action);
  void observationUpdateLog(std::vector<double> obs);
  void observationUpdateLog(std::vector<double>& logProbList, std::vector<double> obs);
  void printLogProbList();
  void printStateList();



}; 
#endif //BAYES_FILTER_H
