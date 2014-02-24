#ifndef REAL_WORLD_H
#define REAL_WORLD_H

#include <vector>
#include "stateStruct.h"
#include "bayesFilter.h"
#include "mechanisms/mechanism.h"
#include "sasUtils.h"

#include <fstream>
#include <sstream>

class RealWorld {

 public:
  std::ofstream outFile_;
  bool writeOutFile_;

  //variables
  BayesFilter filter_;
  Mechanism* mechPtr_; // the mechanism for the simulation if Robot is not used

  std::vector<stateStruct> modelParamPairs_;
  std::vector< std::vector<double> > actionList_;
  sasUtils::mapPairSVS sasList_;
  std::vector< std::vector<double> > workspace_; // robot workspace box
  std::vector<int> numVarTypesPerStateType_; // numbers of each model-param pair
  stateStruct startState_; // starting state

  std::vector<double> action_; // current action in robot space: x,y
  std::vector<double> poseInRbt_; // current state in robot space: x,y
  std::vector<double> obs_; // current observation in robot space: x,y - same as poseInRbt_ right now
  int step_; // the step that the world is on
  int stepsTotal_; // the total number of steps
  int modelNum_; // number of the model to use in the simulation
  int actionSelectionType_; // which type of action selection

  bool useSAS_; // if true, use the SAS list
  bool useRobot_; // if true, use the robot

  //functions
  RealWorld(int modelNum,int numSteps,int writeOutFile,int actionSelectionType,int useRobot);
  ~RealWorld();
  void initMechFree();
  void initMechFixed();
  void initMechRev();
  void initMechPris();
  void initMechRevPrisL();
  void initMechPrisPrisL();

  void initMechRev2();
  void initMechPris2();
  void initMechRevPrisL2();
  void initMechPrisPrisL2();

  bool initializedNearZero();
  void updateFilter(std::vector<double> action,std::vector<double> obs);
  void nextAction();
  void runAction();
  std::vector<double> getObs();
  std::vector<double> getObs(std::vector<double>& stateInRbt);
  void stepWorld();
  void runWorld(int numSteps);
  int runWorldToConf(int numSteps,double confidence);
  double randomDouble();
  double gaussianNoise();
  void printModelParamProbs(std::vector<double> mpProbsLog);

  void writeFileStatesForMATLAB();
  void writeFileInitialData();
  void writeFileStepData();
};
  
#endif //REAL_WORLD_H
