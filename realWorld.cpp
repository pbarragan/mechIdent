// this is an MTV show from the 90s
// The Real World

#include "realWorld.h"
#include "setupUtils.h"
#include "actionSelection.h"
#include "modelUtils.h"
#include "translator.h"

#include "mechanisms/mechFree.h"
#include "mechanisms/mechFixed.h"
#include "mechanisms/mechRev.h"
#include "mechanisms/mechPris.h"
#include "mechanisms/mechRevPrisL.h"
#include "mechanisms/mechPrisPrisL.h"

#include <iostream> // for cout
#include <time.h> // for srand
#include <stdlib.h> // for atoi

// for gaussianNoise()
#include "logUtils.h"
#define _USE_MATH_DEFINES
#include <math.h>

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
RealWorld::RealWorld(int modelNum){
  modelNum_ = modelNum;
  // seed the random number generator once for the real world.
  srand((unsigned)time(NULL));

  // set workspace
  std::vector< std::vector<double> > emptyWorkspace (2,std::vector<double> (2,0.0));
  workspace_ = emptyWorkspace;
  workspace_[0][0]=-0.151;
  workspace_[0][1]=0.151;
  workspace_[1][0]=-0.151;
  workspace_[1][1]=0.151;

  // setup the state list, model-parameter pairs, and log probability list
  setupUtils::setupStates(filter_.stateList_,modelParamPairs_); //initialize states and model parameter pairs
  setupUtils::setupActions(actionList_); //initialize actions

  /*
  std::cout << "actionList_ size before: " << actionList_.size() << std::endl;

  for (size_t k=0;k<actionList_.size();k++){
    std::cout << "action: " << actionList_[k][0] << "," << actionList_[k][1] << " ";
    bool hold1 = actionList_[k][0] < workspace_[0][0];
    bool hold2 = actionList_[k][0] > workspace_[0][1];
    bool hold3 = actionList_[k][1] < workspace_[1][0];
    bool hold4 = actionList_[k][1] > workspace_[1][1];
    std::cout << hold1;
    std::cout << hold2;
    std::cout << hold3;
    std::cout << hold4;
    std::cout << std::endl;
  }
  */
  setupUtils::validateStates(filter_.stateList_,workspace_); // validate states
  setupUtils::validateActions(actionList_,workspace_); // validate actions

  /*
  std::cout << "actionList_ size after: " << actionList_.size() << std::endl;

  std::cout << actionList_[0][0] << "," << actionList_[0][1] << std::endl;
  std::cout << "stateList_ size before: " << filter_.stateList_.size() << std::endl;
  */


  /*
  std::cout << "stateList_ size after: " << filter_.stateList_.size() << std::endl;

  for (size_t w=0;w<filter_.stateList_.size();w++){
    if(filter_.stateList_[w].model==0){
      std::cout << filter_.stateList_[w].vars[0] << "," << filter_.stateList_[w].vars[0] << std::endl;
    }
  }
  */

  setupUtils::setupModelParamPairs(filter_.stateList_,modelParamPairs_); // reinitialize modelParamPairs_ after cutting out states
  setupUtils::setupUniformPrior(filter_.stateList_,filter_.logProbList_,modelParamPairs_); // initliaze probabilities with a uniform distribution

  std::cout << "stateList_ size: " << filter_.stateList_.size() << std::endl; // Print number of states

  // setup either robot or simulator
  useRobot_ = false;
  if (!useRobot_){
    //int modelNum = 5; // Which mechanism to use for the "real world"
    switch(modelNum_){
    case 0:
      initMechFree();
      break;
    case 1:
      initMechFixed();
     break;
    case 2:
      initMechRev();
      break;
    case 3:
      initMechPris();
      break;
    case 4:
      initMechRevPrisL();
      break;
    case 5:
      initMechPrisPrisL();
      break;
    }
    if(initializedNearZero()) std::cout << "We initialized the simulate robot near zero. Heck yeah." << std::endl;
    else std::cout << "\033[1;31mYou screwed up. The simulated robot didn't start with its hand at zero.\033[0m" << std::endl;
    std::cout << "poseInRbt_: " << poseInRbt_[0] << "," <<poseInRbt_[1] << std::endl; // print out the robot's pose
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

////////////////////////////////////////////////////////////////////////////////
//                              Mechanism Section                             //
////////////////////////////////////////////////////////////////////////////////

void RealWorld::initMechFree(){
  // State looks like:
  // Model: 0
  // Params:
  // Vars: x,y in rbt space

  std::cout << "Initializing a MechFree simulation in place of robot" << std::endl;

  // create start state
  stateStruct startState;
  startState.model = 0;
  std::vector<double> stateVars;
  stateVars.push_back(0.0);
  stateVars.push_back(0.0);
  startState.vars = stateVars;

  // check if state is valid
  if (translator::isStateValid(startState,workspace_)) std::cout << "Simulated robot start state is valid. Great job." << std::endl;
  else std::cout << "\033[1;31mSimulated robot start state is NOT valid. This is going to be garbage. Garbage, I tell you!\033[0m" << std::endl;

  // create mechanism
  mechPtr_ = new MechFree();
  mechPtr_->initialize(startState);

  // initialize robot pose
  poseInRbt_ = mechPtr_->stToRbt(startState);
}

void RealWorld::initMechFixed(){
  // State looks like:
  // Model: 1
  // Params: x,y in rbt space
  // Vars: 

  std::cout << "Initializing a MechFixed simulation in place of robot" << std::endl;

  // create start state
  stateStruct startState;
  startState.model = 1;
  std::vector<double> stateParams;
  stateParams.push_back(0.0);
  stateParams.push_back(0.0);
  startState.params = stateParams;

  // check if state is valid
  if (translator::isStateValid(startState,workspace_)) std::cout << "Simulated robot start state is valid. Great job." << std::endl;
  else std::cout << "\033[1;31mSimulated robot start state is NOT valid. This is going to be garbage. Garbage, I tell you!\033[0m" << std::endl;

  // create mechanism
  mechPtr_ = new MechFixed();
  mechPtr_->initialize(startState);

  // initialize robot pose
  poseInRbt_ = mechPtr_->stToRbt(startState);
}

void RealWorld::initMechRev(){
  // State looks like:
  // Model: 2
  // Params: x_pivot,y_pivot in rbt space, r
  // Vars: theta in rbt space

  std::cout << "Initializing a MechRev simulation in place of robot" << std::endl;

  // create start state
  stateStruct startState;
  startState.model = 2;
  std::vector<double> stateParams;
  stateParams.push_back(0.3);
  stateParams.push_back(0.3);
  stateParams.push_back(0.42426);
  startState.params = stateParams;
  std::vector<double> stateVars;
  stateVars.push_back(-2.356);
  startState.vars = stateVars;

  // check if state is valid
  if (translator::isStateValid(startState,workspace_)) std::cout << "Simulated robot start state is valid. Great job." << std::endl;
  else std::cout << "\033[1;31mSimulated robot start state is NOT valid. This is going to be garbage. Garbage, I tell you!\033[0m" << std::endl;

  // create mechanism
  mechPtr_ = new MechRev();
  mechPtr_->initialize(startState);  

  // initialize robot pose
  poseInRbt_ = mechPtr_->stToRbt(startState);
}

void RealWorld::initMechPris(){
  // State looks like:
  // Model: 3
  // Params: x_axis,y_axis,theta_axis in rbt space
  // Vars: d

  std::cout << "Initializing a MechPris simulation in place of robot" << std::endl;

  // create start state
  stateStruct startState;
  startState.model = 3;
  std::vector<double> stateParams;
  stateParams.push_back(-0.16);
  stateParams.push_back(-0.16);
  stateParams.push_back(0.7865);
  startState.params = stateParams;
  std::vector<double> stateVars;
  stateVars.push_back(0.22627);
  startState.vars = stateVars;

  // check if state is valid
  if (translator::isStateValid(startState,workspace_)) std::cout << "Simulated robot start state is valid. Great job." << std::endl;
  else std::cout << "\033[1;31mSimulated robot start state is NOT valid. This is going to be garbage. Garbage, I tell you!\033[0m" << std::endl;

  // create mechanism
  mechPtr_ = new MechPris();
  mechPtr_->initialize(startState);

  // initialize robot pose
  poseInRbt_ = mechPtr_->stToRbt(startState);
}

void RealWorld::initMechRevPrisL(){
  // State looks like:
  // Model: 4
  // Params: x_pivot,y_pivot in rbt space, r, theta_L in rbt space, d_L
  // Vars: theta in rbt space, d

  std::cout << "Initializing a MechRevPrisL simulation in place of robot" << std::endl;

  // create start state
  stateStruct startState;
  startState.model = 4;
  std::vector<double> stateParams;
  stateParams.push_back(-0.6);
  stateParams.push_back(0.0);
  stateParams.push_back(0.4); // 0.431915
  stateParams.push_back(0.0);
  stateParams.push_back(0.2);
  startState.params = stateParams;
  std::vector<double> stateVars;
  stateVars.push_back(0.0);
  stateVars.push_back(0.20);
  startState.vars = stateVars;

  // check if state is valid
  if (translator::isStateValid(startState,workspace_)) std::cout << "Simulated robot start state is valid. Great job." << std::endl;
  else std::cout << "\033[1;31mSimulated robot start state is NOT valid. This is going to be garbage. Garbage, I tell you!\033[0m" << std::endl;

  // create mechanism
  mechPtr_ = new MechRevPrisL();
  mechPtr_->initialize(startState);

  // initialize robot pose
  poseInRbt_ = mechPtr_->stToRbt(startState);
}

void RealWorld::initMechPrisPrisL(){
  // State looks like:
  // Model: 5
  // Params: x_axis2,y_axis2,theta_axis2 in rbt space, d_L2, d_L1
  // Vars: d_2, d_1

  std::cout << "Initializing a MechPrisPrisL simulation in place of robot" << std::endl;

  // create start state
  stateStruct startState;
  startState.model = 5;
  std::vector<double> stateParams;
  stateParams.push_back(-0.1);
  stateParams.push_back(-0.1);
  stateParams.push_back(0.0);
  stateParams.push_back(0.1);
  stateParams.push_back(0.1);
  startState.params = stateParams;
  std::vector<double> stateVars;
  stateVars.push_back(0.1);
  stateVars.push_back(0.1);
  startState.vars = stateVars;

  // check if state is valid
  if (translator::isStateValid(startState,workspace_)) std::cout << "Simulated robot start state is valid. Great job." << std::endl;
  else std::cout << "\033[1;31mSimulated robot start state is NOT valid. This is going to be garbage. Garbage, I tell you!\033[0m" << std::endl;

  // create mechanism
  mechPtr_ = new MechPrisPrisL();
  mechPtr_->initialize(startState);

  // initialize robot pose
  poseInRbt_ = mechPtr_->stToRbt(startState);
}

////////////////////////////////////////////////////////////////////////////////
//                            End Mechanism Section                           //
////////////////////////////////////////////////////////////////////////////////

bool RealWorld::initializedNearZero(){
  double tolerance = 0.001; // tolerance around zero allowed for initialization
  // This relies on a 2D poseInRbt_
  if (poseInRbt_[0]<-tolerance || poseInRbt_[0]>tolerance || poseInRbt_[1]<-tolerance || poseInRbt_[1]>tolerance){
    return false;
  }
  else return true;
}

void RealWorld::updateFilter(std::vector<double> action,std::vector<double> obs){
  // check if you should pass the SAS list to the filter
  if (useSAS_){
    filter_.transitionUpdateLog(action,sasList_);
  }
  else {
    filter_.transitionUpdateLog(action);
  }
  std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! after transition" << std::endl; // DELETE
  filter_.printStatesAndProbs(); // DELETE
  filter_.observationUpdateLog(obs);
  std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! after observation" << std::endl; // DELETE
  filter_.printStatesAndProbs(); // DELETE

}

void RealWorld::nextAction(){
  //actionSelection::chooseActionLog(actionList_,filter_,action_);
  actionSelection::chooseActionSimple(actionList_,step_,action_);
  
  /*
  if(useSAS_){
    actionSelection::chooseActionLog(filter_,actionList_,action_,modelParamPairs_,sasList_);
  }
  else{
    actionSelection::chooseActionLog(filter_,actionList_,action_,modelParamPairs_);
  }
  */
  std::cout << "\033[1;31mstep: \033[0m" << step_ << std::endl;
  std::cout << "\033[1;31maction: \033[0m" << action_[0] << "," << action_[1] << std::endl;
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

    std::cout << "------------------------------------------" << std::endl;
    std::cout << "model: " << tempState.model << std::endl;
    std::cout << "params: " << std::endl;
    for(size_t i=0;i<tempState.params.size();i++){
      std::cout << tempState.params[i] << ",";
    }
    std::cout << std::endl;
    std::cout << "vars: " << std::endl;
    for(size_t i=0;i<tempState.vars.size();i++){
      std::cout << tempState.vars[i] << ",";
    }
    std::cout << std::endl;


    std::cout << "poseInRbt_:" << std::endl; // DELETE
    for (size_t i=0; i<poseInRbt_.size(); i++){
      double X = gaussianNoise();
      std::cout << "before: " << poseInRbt_[i] << std::endl; // DELETE
      poseInRbt_[i]+=X;
      std::cout << "noise: " << X << std::endl; // DELETE
      std::cout << "after: " << poseInRbt_[i] << std::endl; // DELETE
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
    //filter_.printLogProbList();
    std::vector<double> mpProbsLog = modelUtils::calcModelParamProbLog(filter_.stateList_,filter_.logProbList_,modelParamPairs_);
    printModelParamProbs(mpProbsLog);
  }
}

////////////////////////////////////////////////////////////////////////////////
//                               Aux Section                                  //
////////////////////////////////////////////////////////////////////////////////

double RealWorld::randomDouble(){
  double X = ((double)rand()/(double)RAND_MAX);
  return X;
}

double RealWorld::gaussianNoise(){
  double x1 = ((double)rand()/(double)RAND_MAX);
  double x2 = ((double)rand()/(double)RAND_MAX);
  double sig = 0.00001; // standard deviation of noise
  double mu = 0.0; // mean of noise
  return sqrt(-2*logUtils::safe_log(x1))*cos(2*M_PI*x2)*sig+mu;
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
  if (argc > 3) { // We expect 3 arguments: the program name, the model number, the number of iterations
    std::cerr << "Usage: " << argv[0] << "NUMBER OF ITERATIONS" << std::endl;
  }
  else {
    int steps;
    int modelNum;
    if (argc == 1){
      modelNum = 0; // default: free model
      steps = 1; // default: run one step
    }
    else if (argc == 2){
      steps = 1; // default: run one step
    }
    else{
      modelNum = atoi(argv[1]);
      steps = atoi(argv[2]);
    }

    std::cout << "steps: " << steps << std::endl;
    RealWorld world(modelNum);

    //world.nextAction();
    //std::cout << "action: " << world.action_[0] << "," << world.action_[1] << std::endl;

    //world.runWorld(steps);
    
    //testing stuff
    // world.filter_.printStateList();
    //world.filter_.printLogProbList();

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
    std::vector<double> mpProbsLog = modelUtils::calcModelParamProbLog(world.filter_.stateList_,world.filter_.logProbList_,world.modelParamPairs_);
    world.printModelParamProbs(mpProbsLog);
    std::cout << "\033[1;31mbold red text\033[0m\n";
  }
  return 1;
}
