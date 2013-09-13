#ifndef REAL_WORLD_H
#define REAL_WORLD_H

#include <vector>
#include "stateStruct.h"
#include "bayesFilter.h"
#include "mechanisms/mechanism.h"
#include "sasUtils.h"

class RealWorld {

 public:
  //variables
  BayesFilter filter_;
  Mechanism* mechPtr_; // the mechanism for the simulation if Robot is not used

  std::vector<stateStruct> modelParamPairs_;
  std::vector< std::vector<double> > actionList_;
  sasUtils::mapPairSVS sasList_;

  std::vector<double> action_; // current action in robot space: x,y
  std::vector<double> poseInRbt_; // current state in robot space: x,y
  int step_; // the step that the world is on

  bool useSAS_; // if true, use the SAS list
  bool useRobot_; // if true, use the robot

  //functions
  RealWorld();
  ~RealWorld();
  void initMechFree();
  void initMechFixed();
  void initMechRev();
  void initMechPris();
  void initMechRevPrisL();
  void initMechPrisPrisL();
  void updateFilter(std::vector<double> action,std::vector<double> obs);
  void nextAction();
  void runAction();
  std::vector<double> getObs();
  std::vector<double> getObs(std::vector<double>& stateInRbt);
  void stepWorld();
  void runWorld(int numSteps);
  double randomDouble();
  double gaussianNoise();
  void printModelParamProbs(std::vector<double> mpProbsLog);
};
  
#endif //REAL_WORLD_H
