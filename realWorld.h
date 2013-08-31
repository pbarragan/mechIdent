#ifndef REAL_WORLD_H
#define REAL_WORLD_H

#include <vector>
#include "stateStruct.h"
#include "bayesFilter.h"
#include "mechanisms/mechanism.h"

class RealWorld {

 public:
  //variables
  BayesFilter filter_;
  Mechanism* mechPtr_; // the mechanism for the simulation if Robot is not used
  std::vector<stateStruct> modelParamPairs_;
  std::vector< std::vector<double> > actionList_;
  std::vector<double> action_; // current action
  std::vector<double> stateInRbt_; // current state in robot space
  bool useRobot_; // if true, use the robot
  int step_; // the step that the world is on

  //functions
  RealWorld();
  void initMechFree();
  void updateFilter(std::vector<double> action,std::vector<double> obs);
  void nextAction();
  void runAction();
  std::vector<double> getObs();
  std::vector<double> getObs(std::vector<double>& stateInRbt);
  void stepWorld();
  void runWorld(int numSteps);
  double randomDouble();
  void printModelParamProbs(std::vector<double> mpProbsLog);
}
  
#endif //REAL_WORLD_H
