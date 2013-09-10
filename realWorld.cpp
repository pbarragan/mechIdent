// this is an MTV show from the 90s
// The Real World

#include "realWorld.h"
#include "setupUtils.h"
#include "actionSelection.h"
#include "modelUtils.h"

#include "mechanisms/mechFree.h"
#include "mechanisms/mechFixed.h"

#include <iostream> // for cout
#include <time.h> // for srand
#include <stdlib.h> // for atoi

// temporary
#include "logUtils.h"

//calcModelProbLog - modelUtils X
//chooseActionLog - realWorld / actionSelection
//createValueList - setupUtils X
//recruseList - setupUtils X

//setupActions - setupUtils X
//setupStates - setupUtils X
//setupUniformPrior - setupUtils X
//getObs - realWorld
//populateSAS - setupUtils
//setupSAS - setupUtils

//To DO:
//setup the function in modelUtils to use the modelParamPairs. overload it. X

// make the getObs function for the simulations
//0) choose action
//1) run the real world
//2) updateFilter
//3) repeat


//Constructor
RealWorld::RealWorld(){
  // seed the random number generator once for the real world.
  srand((unsigned)time(NULL));

  // setup the state list, model-parameter pairs, and log probability list
  setupUtils::setupStates(filter_.stateList_,modelParamPairs_); //initialize states and model parameter pairs
  setupUtils::setupUniformPrior(filter_.stateList_,filter_.logProbList_,modelParamPairs_); //initliaze probabilities with a uniform distribution
  setupUtils::setupActions(actionList_); //initialize actions
  
  // setup either robot or simulator
  useRobot_ = false;
  if (!useRobot_){
    initMechFree();
  }

  // use the SAS list?
  useSAS_ = true;
  if (useSAS_){
    bool overwriteCSV = true;
    sasUtils::setupSAS(sasList_,filter_.stateList_,actionList_,overwriteCSV);
  }

  // print model probabilities before any action is taken
  std::vector<double> mpProbsLog = modelUtils::calcModelParamProbLog(filter_.stateList_,filter_.logProbList_,modelParamPairs_);
  printModelParamProbs(mpProbsLog);
}

//Destructor
RealWorld::~RealWorld(){
  delete mechPtr_;
}

void RealWorld::initMechFree(){
  // create start state
  stateStruct startState;
  startState.model = 0;
  std::vector<double> stateVars;
  stateVars.push_back(0.2);
  stateVars.push_back(0.2);
  startState.vars = stateVars;
  
  // initialize robot pose
  poseInRbt_ = stateVars;

  // create mechanism
  mechPtr_ = new MechFree();
  mechPtr_->initialize(startState);
}

void RealWorld::updateFilter(std::vector<double> action,std::vector<double> obs){
  // check if you should pass the SAS list to the filter
  if (useSAS_){
    filter_.transitionUpdateLog(action,sasList_);
  }
  else {
    filter_.transitionUpdateLog(action);
  }
  filter_.observationUpdateLog(obs);
}

void RealWorld::nextAction(){
  //actionSelection::chooseActionLog(actionList_,filter_,action_);
  //actionSelection::chooseActionSimple(actionList_,step_,action_);
  if(useSAS_){
    actionSelection::chooseActionLog(filter_,actionList_,action_,modelParamPairs_,sasList_);
  }
  else{
    actionSelection::chooseActionLog(filter_,actionList_,action_,modelParamPairs_);
  }
  std::cout << "action: " << action_[0] << "," << action_[1] << std::endl;
}

// Run action in either simulation or real world
void RealWorld::runAction(){
  if (!useRobot_){
    stateStruct tempState;
    tempState = mechPtr_->simulate(action_);
    poseInRbt_ = mechPtr_->stToRbt(tempState); // state of the robot
    // The state known to the robot should have some noise on it. 
    // The simulation returns the nominal answer.
    // Noise should be added such that the "true" (nominal) position is unknown
    for (size_t i=0; i<poseInRbt_.size(); i++){
      double X=randomDouble();
      poseInRbt_[i]+=0.001*X;
    }
  }
}

//overload this function
std::vector<double> RealWorld::getObs(){
  return getObs(poseInRbt_);
}

//overload this function
std::vector<double> RealWorld::getObs(std::vector<double>& poseInRbt){
  // This is very specific to the simulation
  std::vector<double> obs = mechPtr_->rbtToObs(poseInRbt);
  return obs;
}

void RealWorld::stepWorld(){
  nextAction(); // 1. choose an action
  runAction(); // 2. run action on the world
  std::vector<double> tempObs = getObs(); // 3. get observation from the world
  updateFilter(action_,tempObs);
}

void RealWorld::runWorld(int numSteps){
  for (size_t i=0;i<numSteps;i++){
    step_=i;
    stepWorld();
    filter_.printLogProbList();
    std::vector<double> mpProbsLog = modelUtils::calcModelParamProbLog(filter_.stateList_,filter_.logProbList_,modelParamPairs_);
    printModelParamProbs(mpProbsLog);
  }
}

////////////////////////////////////////////////////////////////////////////////
//                               Aux Section                                  //
////////////////////////////////////////////////////////////////////////////////

double RealWorld::randomDouble(){
  double X=((double)rand()/(double)RAND_MAX);
  return X;
}

void RealWorld::printModelParamProbs(std::vector<double> mpProbsLog){
  std::cout << "Model-param pairs and probs:" << std::endl;
  for (size_t i=0;i<modelParamPairs_.size();i++){
    std::cout << "Model: " << modelParamPairs_[i].model << std::endl;
    std::cout << "Params: ";
    for (size_t j = 0; j<modelParamPairs_[i].params.size(); j++) {
      std::cout << modelParamPairs_[i].params[j] << ',';
    }
    std::cout << std::endl;
    std::cout << "Prob: " << mpProbsLog[i] << std::endl;
  }
}

////////////////////////////////////////////////////////////////////////////////
//                             End Aux Section                                //
////////////////////////////////////////////////////////////////////////////////

void printVect(std::vector<double> vect){
  for (size_t i=0; i<vect.size(); i++){
    std::cout << vect[i] << ",";
  }
  std::cout << std::endl;
}

int main(int argc, char* argv[])
{
  if (argc > 2) { // We expect 2 arguments: the program name, the number of iterations
    std::cerr << "Usage: " << argv[0] << "NUMBER OF ITERATIONS" << std::endl;
  }
  else {
    int steps;
    if (argc == 1){
      steps = 1; // default: run one step
    }
    else{
      steps = atoi(argv[1]);
    }
    std::cout << "steps: " << steps << std::endl;
    RealWorld world;

    //world.nextAction();
    //std::cout << "action: " << world.action_[0] << "," << world.action_[1] << std::endl;

    //world.runWorld(steps);
    
    //testing stuff
    //world.filter_.printStateList();

    /*
    std::cout << "modelParamPairs: " << std::endl;
    for (size_t i=0;i<world.modelParamPairs_.size();i++){
      std::cout << world.modelParamPairs_[i].model << std::endl;
      printVect(world.modelParamPairs_[i].params);
    }
    std::cout << "probList: " << std::endl;
    printVect(world.filter_.logProbList_);
    for (size_t i = 0;i<world.filter_.logProbList_.size();i++){
      std::cout << logUtils::safe_exp(world.filter_.logProbList_[i]) << std::endl;
    }
    std::cout << "actionList: " << std::endl;
    for (size_t i=0;i<world.actionList_.size();i++){
      printVect(world.actionList_[i]);
    }

    std::cout << "sasList size: " << world.sasList_.size() << std::endl;
    if(world.useSAS_){
      sasUtils::mapPairSVS_it mapIt = world.sasList_.begin();
      stateStruct tmpPrevState = mapIt->first.first;
      std::vector<double> tmpAction = mapIt->first.second;
      stateStruct tmpNextState = mapIt->second;
      
      std::cout << "model" << tmpPrevState.model << std::endl;
      std::cout << "params" << tmpPrevState.params.size() << std::endl;
      for (size_t i=0;i<tmpPrevState.params.size();i++){
	std::cout << tmpPrevState.params[i] << std::endl;
      }
      std::cout << "vars" << tmpPrevState.vars.size() << std::endl;
      for (size_t i=0;i<tmpPrevState.vars.size();i++){
	std::cout << tmpPrevState.vars[i] << std::endl;
      }
    }
    */
    world.runWorld(steps);
  }
  return 1;
}
