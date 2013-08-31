//this is an MTV show from the 90s
//The Real World

#include "realWorld.h"
#include "setupUtils.h"
#include "actionSelection.h"
#include "modelUtils.h" 

#include <iostream> // for cout
#include <time.h> // for srand
#include <stdlib.h> // for atoi

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
}

void RealWorld::initMechFree(){
  // create start state
  stateStruct startState;
  startState.model = 0;
  std::vector<double> stateVars;
  stateVars.push_back(0.0);
  stateVars.push_back(0.0);
  startState.vars = stateVars;

  // create mechanism
  mechPtr_ = new MechFree();
  mechPtr_->initialize(startState);
}

void RealWorld::updateFilter(std::vector<double> action,std::vector<double> obs){
  filter_.transitionUpdateLog(action);
  filter_.observationUpdateLog(obs);
}

void RealWorld::nextAction(){
  actionSelection::chooseActionLog(actionList_,filter_,action_);
}

// Run action in either simulation or real world
void RealWorld::runAction(){
  if (!useRobot){
    stateStruct tempState;
    tempState = mechPtr_->simulate(action_);
    stateInRbt_ = mechPtr_->stToRbt(tempState); // state of the robot
    // The state known to the robot should have some noise on it. 
    // The simulation returns the nominal answer.
    // Noise should be added such that the "true" (nominal) position is unknown
    for (size_t i=0; i<stateInRbt_.size(); i++){
      double X=randomDouble();
      stateInRbt_[i]+=0.001*X;
    }
  }
}

//overload this function
std::vector<double> RealWorld::getObs(){
  return getObs(stateInRbt_);
}

//overload this function
std::vector<double> RealWorld::getObs(std::vector<double>& stateInRbt){
  std::vector<double> obs = mechPtr_->rbtToObs(stateInRbt);
  return obs;
}

void RealWorld::stepWorld(){
  nextAction(); // 1. choose an action
  runAction();  // 2. run action on the world
  std::vector<double> tempObs = getObs();     // 3. get observation from the world
  updateFilter(action_,tempObs);
}

void RealWorld::runWorld(int numSteps){
  for (size_t i=0;i<numSteps;i++){
    step_=i;
    stepWorld();
    std::vector<double> mpProbsLog = modelUtils::calcModelParamProbLog(filter_.stateList_,filter_.logProbList_,modelParamPairs_);
    printModelParamProbs(mpProbsLog)
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
  for (size_t i;i<modelParamPairs_.size();i++){
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

int main(int argc, char* argv[])
{
    if (argc > 2) { // We expect 2 arguments: the program name, the number of iterations
        std::cerr << "Usage: " << argv[0] << "NUMBER OF ITERATIONS" << std::endl;
    }
    else {
      int steps = atoI(argv[1]);
      RealWorld world;
      world.runWorld(steps);
    }
    return 1;
}
