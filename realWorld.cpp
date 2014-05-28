// this is an MTV show from the 90s
// The Real World

#include "globalVars.h"

#include "realWorld.h"
#include "setupUtils.h"
#include "actionSelection.h"
#include "modelUtils.h"
#include "translator.h"
#include "robotComm.h"

#include "mechanisms/mechFree.h"
#include "mechanisms/mechFixed.h"
#include "mechanisms/mechRev.h"
#include "mechanisms/mechPris.h"
#include "mechanisms/mechRevPrisL.h"
#include "mechanisms/mechPrisPrisL.h"

#include <iostream> // for cout
#include <iomanip> // for setprecision
#include <time.h> // for srand
#include <stdlib.h> // for atoi
#include <string>
#include <ctime>
#include <algorithm>

// for gaussianNoise()
#include "logUtils.h"
#define _USE_MATH_DEFINES
#include <math.h>

// for averaging
#include <numeric>      // std::accumulate

// for timing
//#include <sys/time.h>

//Constructor
RealWorld::RealWorld(int modelNum,int numSteps,int writeOutFile,int actionSelectionType,int useRobot) : mechPtr_(NULL) {
  writeOutFile_ = writeOutFile;
  stepsTotal_ = numSteps;
  modelNum_ = modelNum;
  actionSelectionType_ = actionSelectionType;
  useRobot_ = useRobot;

  // get today's date and time
  // current date/time based on current system
  time_t now = time(0);
  
  // convert now to string form
  char* dt = ctime(&now);
  std::string dateString = dt;
  dateString = dateString.substr(0,dateString.size()-1);
  std::replace(dateString.begin(),dateString.end(),' ','_');
  std::replace(dateString.begin(),dateString.end(),':','_');

  if(writeOutFile_){ 
    std::stringstream ss;
    ss << "data/data" << modelNum << dateString << ".txt";
    std::string saveName = ss.str();
    outFile_.open(saveName.c_str());
  }

  // seed the random number generator once for the real world.
  srand((unsigned)time(NULL));

  //set numVarTypes vector to empty vector
  //std::vector<int> emptyIntVect (1,0);
  //numVarTypesPerStateType_ = emptyIntVect;

  // set workspace
  std::vector< std::vector<double> > emptyWorkspace (2,std::vector<double> (2,0.0));
  workspace_ = emptyWorkspace;
  workspace_[0][0]=WORKSPACE[0][0];
  workspace_[0][1]=WORKSPACE[0][1];
  workspace_[1][0]=WORKSPACE[1][0];
  workspace_[1][1]=WORKSPACE[1][1];

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
  if (!RELATIVE){
    setupUtils::validateActions(actionList_,workspace_); // validate actions
  }
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

  setupUtils::setupModelParamPairs(filter_.stateList_,modelParamPairs_,numVarTypesPerStateType_); // reinitialize modelParamPairs_ after cutting out states

  //setupUtils::setupUniformPrior(filter_.stateList_,filter_.logProbList_,modelParamPairs_); // initliaze probabilities with a uniform distribution

  setupUtils::setupGaussianPrior(filter_.stateList_,filter_.logProbList_,modelParamPairs_); // initliaze probabilities with a gaussian distribution
  std::cout << "stateList_ size: " << filter_.stateList_.size() << std::endl; // Print number of states

  // setup either robot or simulator
  //useRobot_ = false;
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
    case 6:
      initMechRev2();
      break;
    case 7:
      initMechPris2();
      break;
    case 8:
      initMechRevPrisL2();
      break;
    case 9:
      initMechPrisPrisL2();
      break;
    }
  }
  else if (useRobot_){
    mechPtr_ = new Mechanism();
    std::vector<double> zeroVect (2,0.0);
    poseInRbt_ = zeroVect; // relies on 2D - robot always starts at 0.0
  }

  // Print the initial pose of the robot
  if(initializedNearZero()) std::cout << "We initialized the simulate robot near zero. Heck yeah." << std::endl;
  else std::cout << "\033[1;31mYou screwed up. The simulated robot didn't start with its hand at zero.\033[0m" << std::endl;
  std::cout << "poseInRbt_: " << poseInRbt_[0] << "," <<poseInRbt_[1] << std::endl; // print out the robot's pose

  // use the SAS list?
  useSAS_ = true;
  if (useSAS_){
    bool overwriteCSV = true;
    std::string fileName;
    if (RELATIVE){
    fileName = "files/sasSaveRel.txt";
    }
    else{
    fileName = "files/sasSaveAbs.txt";
    }
    //fileName = "files/sasSaveRel.txt";
    sasUtils::setupSAS(sasList_,filter_.stateList_,actionList_,overwriteCSV,fileName);
  }

  // print model probabilities before any action is taken
  std::vector<double> mpProbsLog = modelUtils::calcModelParamProbLog(filter_.stateList_,filter_.logProbList_,modelParamPairs_);
  printModelParamProbs(mpProbsLog);

  /*
  // trying to debug model 4
  std::cout << ",,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,Model 4 stuff" << std::endl;
  for(size_t i=0;i<filter_.stateList_.size();i++){
    if(filter_.stateList_[i].model==4){
      std::vector<double> obs = translator::translateStToObs(filter_.stateList_[i]);
      std::cout << obs[0] << "," << obs[1] << std::endl;
    }
  }
  // trying to debug model 0
  std::cout << ",,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,Model 0 stuff" << std::endl;
  for(size_t i=0;i<filter_.stateList_.size();i++){
    if(filter_.stateList_[i].model==0){
      std::vector<double> obs = translator::translateStToObs(filter_.stateList_[i]);
      std::cout << obs[0] << "," << obs[1] << std::endl;
    }
  }
  */

  if(false){
    // File style
    std::stringstream ss2;
    ss2 << "data/stateDisplay" << dateString << ".txt";
    std::string statesFileName = ss2.str();

    std::ofstream statesFile(statesFileName.c_str());
    for(size_t i=0;i<filter_.stateList_.size();i++){
      std::vector<double> obs = translator::translateStToObs(filter_.stateList_[i]);
      statesFile << filter_.stateList_[i].model << "," << obs[0] << "," << obs[1] << ";\n";
    }
    statesFile.close();
  }
  
  if(writeOutFile_){
    writeFileInitialData(); // write the initial data to the file
  }

  if(false){
    writeFileStatesForMATLAB(); // write state file for visualization in MATLAB
  }

}

//Destructor
RealWorld::~RealWorld(){
  delete mechPtr_;
  if(writeOutFile_){
    outFile_ << "\n";
    outFile_.close();
  }
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
  startState_=startState;
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
  startState_=startState;
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
  stateParams.push_back(0.396); // 0.3111
  stateParams.push_back(0.396); // 0.3111
  stateParams.push_back(0.56); // 0.44
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
  startState_=startState;
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
  stateParams.push_back(0.0);
  stateParams.push_back(0.226274);
  stateParams.push_back(-1.570796);
  startState.params = stateParams;
  std::vector<double> stateVars;
  stateVars.push_back(0.226274);
  startState.vars = stateVars;

  // check if state is valid
  if (translator::isStateValid(startState,workspace_)) std::cout << "Simulated robot start state is valid. Great job." << std::endl;
  else std::cout << "\033[1;31mSimulated robot start state is NOT valid. This is going to be garbage. Garbage, I tell you!\033[0m" << std::endl;

  // create mechanism
  mechPtr_ = new MechPris();
  mechPtr_->initialize(startState);
  startState_=startState;
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
  stateParams.push_back(0.27); // -0.6 // ICRA 2014 and Vid1 - -0.2
  stateParams.push_back(0.0); // 0.0
  stateParams.push_back(0.17); // 0.4 // ICRA 2014 and Vid1 - 0.1
  stateParams.push_back(-3.14159); // 0.0 // ICRA 2014 and Vid1 - 0.0
  stateParams.push_back(0.1); // 0.2
  startState.params = stateParams;
  std::vector<double> stateVars;
  stateVars.push_back(-3.14159); // 0.0 // ICRA 2014 and Vid1 - 0.0
  stateVars.push_back(0.10); // 0.20 // ICRA 2014 and Vid1 - 0.10
  startState.vars = stateVars;

  // check if state is valid
  if (translator::isStateValid(startState,workspace_)) std::cout << "Simulated robot start state is valid. Great job." << std::endl;
  else std::cout << "\033[1;31mSimulated robot start state is NOT valid. This is going to be garbage. Garbage, I tell you!\033[0m" << std::endl;

  // create mechanism
  mechPtr_ = new MechRevPrisL();
  mechPtr_->initialize(startState);
  startState_=startState;
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
  startState_=startState;
  // initialize robot pose
  poseInRbt_ = mechPtr_->stToRbt(startState);
}

////////////////////////////////////////////////////////////////////////////////
//                            End Mechanism Section                           //
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//                           Extra Mechanism Section                          //
////////////////////////////////////////////////////////////////////////////////

void RealWorld::initMechRev2(){
  // State looks like:
  // Model: 6
  // Params: x_pivot,y_pivot in rbt space, r
  // Vars: theta in rbt space

  std::cout << "Initializing a MechRev2 simulation in place of robot" << std::endl;

  // create start state
  stateStruct startState;
  startState.model = 2;
  std::vector<double> stateParams;
  stateParams.push_back(0.396); // 0.3111
  stateParams.push_back(-0.396); // -0.3111
  stateParams.push_back(0.56); // 0.44
  startState.params = stateParams;
  std::vector<double> stateVars;
  stateVars.push_back(2.356);
  startState.vars = stateVars;

  // check if state is valid
  if (translator::isStateValid(startState,workspace_)) std::cout << "Simulated robot start state is valid. Great job." << std::endl;
  else std::cout << "\033[1;31mSimulated robot start state is NOT valid. This is going to be garbage. Garbage, I tell you!\033[0m" << std::endl;

  // create mechanism
  mechPtr_ = new MechRev();
  mechPtr_->initialize(startState);  
  startState_=startState;
  // initialize robot pose
  poseInRbt_ = mechPtr_->stToRbt(startState);
}

void RealWorld::initMechPris2(){
  // State looks like:
  // Model: 7
  // Params: x_axis,y_axis,theta_axis in rbt space
  // Vars: d

  std::cout << "Initializing a MechPris2 simulation in place of robot" << std::endl;

  // create start state
  stateStruct startState;
  startState.model = 3;
  std::vector<double> stateParams;
  stateParams.push_back(0.226274);
  stateParams.push_back(0.0);
  stateParams.push_back(-3.14159);
  startState.params = stateParams;
  std::vector<double> stateVars;
  stateVars.push_back(0.226274);
  startState.vars = stateVars;

  // check if state is valid
  if (translator::isStateValid(startState,workspace_)) std::cout << "Simulated robot start state is valid. Great job." << std::endl;
  else std::cout << "\033[1;31mSimulated robot start state is NOT valid. This is going to be garbage. Garbage, I tell you!\033[0m" << std::endl;

  // create mechanism
  mechPtr_ = new MechPris();
  mechPtr_->initialize(startState);
  startState_=startState;
  // initialize robot pose
  poseInRbt_ = mechPtr_->stToRbt(startState);
}

void RealWorld::initMechRevPrisL2(){
  // State looks like:
  // Model: 8
  // Params: x_pivot,y_pivot in rbt space, r, theta_L in rbt space, d_L
  // Vars: theta in rbt space, d

  std::cout << "Initializing a MechRevPrisL2 simulation in place of robot" << std::endl;

  // create start state
  stateStruct startState;
  startState.model = 4;
  std::vector<double> stateParams;
  stateParams.push_back(-0.27); // -0.6 // ICRA 2014 and Vid1 - -0.2
  stateParams.push_back(0.0); // 0.0
  stateParams.push_back(0.17); // 0.4 // ICRA 2014 and Vid1 - 0.1
  stateParams.push_back(0.0); // 0.0 // ICRA 2014 and Vid1 - 0.0
  stateParams.push_back(0.1); // 0.2
  startState.params = stateParams;
  std::vector<double> stateVars;
  stateVars.push_back(0.0); // 0.0 // ICRA 2014 and Vid1 - 0.0
  stateVars.push_back(0.10); // 0.20 // ICRA 2014 and Vid1 - 0.10
  startState.vars = stateVars;

  // check if state is valid
  if (translator::isStateValid(startState,workspace_)) std::cout << "Simulated robot start state is valid. Great job." << std::endl;
  else std::cout << "\033[1;31mSimulated robot start state is NOT valid. This is going to be garbage. Garbage, I tell you!\033[0m" << std::endl;

  // create mechanism
  mechPtr_ = new MechRevPrisL();
  mechPtr_->initialize(startState);
  startState_=startState;
  // initialize robot pose
  poseInRbt_ = mechPtr_->stToRbt(startState);
}

void RealWorld::initMechPrisPrisL2(){
  // State looks like:
  // Model: 9
  // Params: x_axis2,y_axis2,theta_axis2 in rbt space, d_L2, d_L1
  // Vars: d_2, d_1

  std::cout << "Initializing a MechPrisPrisL2 simulation in place of robot" << std::endl;

  // create start state
  stateStruct startState;
  startState.model = 5;
  std::vector<double> stateParams;
  stateParams.push_back(0.1);
  stateParams.push_back(0.1);
  stateParams.push_back(-3.14159);
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
  startState_=startState;
  // initialize robot pose
  poseInRbt_ = mechPtr_->stToRbt(startState);
}

////////////////////////////////////////////////////////////////////////////////
//                         End Extra Mechanism Section                        //
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

  std::cout << "Printing Log Probablity List for Prismatic 3 before:" << std::endl;
  std::cout << std::fixed;
  std::cout << std::setprecision(16);
  for (size_t ii = 0; ii<filter_.logProbList_.size(); ii++) {
    if(filter_.stateList_[ii].model==5){
      std::cout << filter_.logProbList_[ii] <<std::endl;
    }
  }


  // check if you should pass the SAS list to the filter
  if (useSAS_){
    filter_.transitionUpdateLog(action,sasList_);
  }
  else {
    filter_.transitionUpdateLog(action);
  }

  std::cout << "Printing Log Probablity List for Prismatic 3 after:" << std::endl;
  std::cout << std::fixed;
  std::cout << std::setprecision(16);
  for (size_t ii = 0; ii<filter_.logProbList_.size(); ii++) {
    if(filter_.stateList_[ii].model==5){
    std::cout << filter_.logProbList_[ii] <<std::endl;
    }
  }

  //std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! after transition" << std::endl; // DELETE
  //filter_.printStatesAndProbs(); // DELETE
  filter_.observationUpdateLog(obs);
  //std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! after observation" << std::endl; // DELETE
  //filter_.printStatesAndProbs(); // DELETE

}

void RealWorld::nextAction(){
  //actionSelection::chooseActionLog(actionList_,filter_,action_);
  //actionSelection::chooseActionSimple(actionList_,step_,action_);
  int whichSelectionType = actionSelectionType_; // which action selection scheme should we use

  if (RELATIVE){
    // relative
    std::cout << "Relative actions:" << std::endl;
    if (whichSelectionType == 0){
      std::cout << "Simple Action Selection:" << std::endl;
      actionSelection::chooseActionSimpleRel(actionList_,step_,action_,poseInRbt_,workspace_);
    }
    else if (whichSelectionType == 1){
      std::cout << "Random Action Selection:" << std::endl;
      actionSelection::chooseActionRandomRel(actionList_,action_,poseInRbt_,workspace_);
    }
    else if (whichSelectionType == 2){
      std::cout << "Entropy Action Selection:" << std::endl;
      if(useSAS_){
	actionSelection::chooseActionLogRel(filter_,actionList_,action_,modelParamPairs_,sasList_,poseInRbt_,workspace_);
      }
      else{
	actionSelection::chooseActionLogRel(filter_,actionList_,action_,modelParamPairs_,poseInRbt_,workspace_);
      }
    }
    else if (whichSelectionType == 3){
      std::cout << "OG Action Selection:" << std::endl;
      if(useSAS_){
	actionSelection::chooseActionOGRel(filter_,actionList_,action_,modelParamPairs_,sasList_,poseInRbt_,workspace_);
      }
      else{
	actionSelection::chooseActionOGRel(filter_,actionList_,action_,modelParamPairs_,poseInRbt_,workspace_);
      }
    }
  }
  else{
    // Absolute
    std::cout << "Absolute actions:" << std::endl;
    if (whichSelectionType == 0){
      std::cout << "Simple Action Selection:" << std::endl;
      actionSelection::chooseActionSimple(actionList_,step_,action_);
    }
    else if (whichSelectionType == 1){
      std::cout << "Random Action Selection:" << std::endl;
      actionSelection::chooseActionRandom(actionList_,action_);
    }
    else if (whichSelectionType == 2){
      std::cout << "Entropy Action Selection:" << std::endl;
      if(useSAS_){
	actionSelection::chooseActionLog(filter_,actionList_,action_,modelParamPairs_,sasList_);
      }
      else{
	actionSelection::chooseActionLog(filter_,actionList_,action_,modelParamPairs_);
      }
    }
    else if (whichSelectionType == 3){
      std::cout << "OG Action Selection:" << std::endl;
      if(useSAS_){
	actionSelection::chooseActionOG(filter_,actionList_,action_,modelParamPairs_,sasList_);
      }
      else{
	actionSelection::chooseActionOG(filter_,actionList_,action_,modelParamPairs_);
      }
    }
  }
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

    /*
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
    */

    std::cout << "poseInRbt_:" << std::endl; // DELETE
    for (size_t i=0; i<poseInRbt_.size(); i++){
      double X = gaussianNoise(); //remove the noise for a second
      //double X = 0.0;
      std::cout << "before: " << poseInRbt_[i] << std::endl; // DELETE
      poseInRbt_[i]+=X;
      std::cout << "noise: " << X << std::endl; // DELETE
      std::cout << "after: " << poseInRbt_[i] << std::endl; // DELETE
    }
  }
  else if (useRobot_){
    // change the action the robot is supposed to use to make sure it's absolute
    std::vector<double> actionInRbt (2,0.0);
    if (RELATIVE){
      for(size_t i=0; i<action_.size(); i++){
	actionInRbt[i]=poseInRbt_[i]+action_[i];
      }
    }
    else{
      actionInRbt = action_;
    }
    // Send action to robot and wait
    if (robotComm::sendRequest(actionInRbt)){
      std::cout << "Send action to robot." << std::endl;
      // Get response. Set pose of robot. getObs() can then be used
      if (robotComm::getResponse(poseInRbt_)){ 
	std::cout << "Got pose from robot." << std::endl;
	std::cout << "poseInRbt_ after action: " << poseInRbt_[0] << "," << poseInRbt_[1] << std::endl;
      }
      else std::cout << "\033[1;31mFailed to get pose from robot.\033[0m" << std::endl;
    }
    else std::cout << "\033[1;31mFailed to send action to robot.\033[0m" << std::endl;
  }
}

//overload this function
std::vector<double> RealWorld::getObs(){
  return getObs(poseInRbt_);
}

//overload this function
std::vector<double> RealWorld::getObs(std::vector<double>& poseInRbt){
  // This can be used for simulation or robot
  std::vector<double> obs = mechPtr_->rbtToObs(poseInRbt);
  obs_ = obs;
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
    if(writeOutFile_){
      writeFileStepData(); // write the step data to the file
    }
  }
  //outFile << "did this work\n";
}

int RealWorld::runWorldToConf(int numSteps,double confidence){
  // run the world until confidence in a single model reaches threshold or number of steps runs out
  int stepsNeeded = 0;
  bool end=false;
  for (size_t i=0;i<numSteps;i++){
    step_=i;
    stepWorld();
    //filter_.printLogProbList();
    std::vector<double> mpProbsLog = modelUtils::calcModelParamProbLog(filter_.stateList_,filter_.logProbList_,modelParamPairs_);
    //printModelParamProbs(mpProbsLog);
    if(writeOutFile_){
      writeFileStepData(); // write the step data to the file
    }
    stepsNeeded=i;
    // check if you've reached the confidence level and exit if you have
    for (size_t j=0;j<mpProbsLog.size();j++){
      if (mpProbsLog[j]>=confidence){
	std::cout << "\033[1;31mBest Guess: \033[0m" << j << std::endl;
	end=true;
	break;
      }
    }
    if (end){
      break;
    }
  }
  return stepsNeeded+1;
  //outFile << "did this work\n";
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
  double sig = 0.01; // standard deviation of noise - it worked when it was 0.00001 - still worked with 0.01
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
    std::cout << "Number Var Types: " << numVarTypesPerStateType_[i] << std::endl;
  }
}

////////////////////////////////////////////////////////////////////////////////
//                             End Aux Section                                //
////////////////////////////////////////////////////////////////////////////////

void RealWorld::writeFileStatesForMATLAB(){
  // Write file to visualize states in MATLAB
  std::ofstream stateFile;
  std::string stateFileName = "files/stateFileForVisualization.txt";
  stateFile.open(stateFileName.c_str());

  std::vector<double> tempCartPosInRbt;
  for(size_t i=0;i<filter_.stateList_.size();i++){
    stateFile << filter_.stateList_[i].model;
    tempCartPosInRbt = translator::translateStToRbt(filter_.stateList_[i]);
    for(size_t j=0;j<tempCartPosInRbt.size();j++){
      stateFile << "," << tempCartPosInRbt[j]; 
    }
    stateFile << ";\n";
  }
  stateFile.close();
}

void RealWorld::writeFileInitialData(){
  // Write action type
  outFile_ << "Action type:\n";
  outFile_ << RELATIVE << "\n";
  // Write different things depending on whether using the robot or not
  if (!useRobot_){
    // use simulation
    //
    outFile_ << "Real Model:\n";
    outFile_ << modelNum_ << "\n";
    //
    outFile_ << "Real Params:\n";
    for (size_t i=0;i<startState_.params.size();i++){
      outFile_ << startState_.params[i] << ",";
    }
    outFile_ << "\n";
    //
    outFile_ << "Real Vars:\n";
    for (size_t i=0;i<startState_.vars.size();i++){
      outFile_ << startState_.vars[i] << ",";
    }
    outFile_ << "\n";
  }
  else if (useRobot_){
    // using robot
        //
    outFile_ << "Real Model (if user inputted correctly):\n";
    outFile_ << modelNum_ << "\n";
    //
    outFile_ << "Real Params:\n";
    outFile_ << "unknown" << "\n";
    //
    outFile_ << "Real Vars:\n";
    outFile_ << "unknown" << "\n";
  }
  //
  outFile_ << "Num States:\n";
  outFile_ << filter_.stateList_.size() << "\n";	
  //
  outFile_ << "States:\n";
  for (size_t j=0;j<filter_.stateList_.size();j++){
    //
    outFile_ << "Model:\n";
    outFile_ << filter_.stateList_[j].model << "\n";
    //
    outFile_ << "Params:\n";
    for (size_t i=0;i<filter_.stateList_[j].params.size();i++){
      outFile_ << filter_.stateList_[j].params[i] << ",";
    }
    outFile_ << "\n";
    //
    outFile_ << "Vars:\n";
    for (size_t i=0;i<filter_.stateList_[j].vars.size();i++){
      outFile_ << filter_.stateList_[j].vars[i] << ",";
    }
    outFile_ << "\n";
  }
  //
  outFile_ << "States in Rbt:\n";
  //
  std::vector<double> tempCartPosInRbt;
  for(size_t i=0;i<filter_.stateList_.size();i++){
    outFile_ << filter_.stateList_[i].model;
    tempCartPosInRbt = translator::translateStToRbt(filter_.stateList_[i]);
    for(size_t j=0;j<tempCartPosInRbt.size();j++){
      outFile_ << "," << tempCartPosInRbt[j]; 
    }
    outFile_ << "\n";
  }
  //
  outFile_ << "Num Actions:\n";
  outFile_ << actionList_.size() << "\n";	
  //
  outFile_ << "Actions:\n";
  for (size_t i=0;i<actionList_.size();i++){
    for (size_t j=0;j<actionList_[i].size();j++){
      outFile_ << actionList_[i][j]<< ",";
    }
    outFile_ << "\n";
  }
  //
  outFile_ << "Num Model-Param Pairs:\n";
  outFile_ << modelParamPairs_.size() << "\n";	
  //
  outFile_ << "Model-Param Pairs:\n";
  for (size_t j=0;j<modelParamPairs_.size();j++){
    //
    outFile_ << "Model:\n";
    outFile_ << modelParamPairs_[j].model << "\n";
    //
    outFile_ << "Params:\n";
    for (size_t i=0;i<modelParamPairs_[j].params.size();i++){
      outFile_ << modelParamPairs_[j].params[i] << ",";
    }
    outFile_ << "\n";
    //
    outFile_ << "Num Var Types:\n";
    outFile_ << numVarTypesPerStateType_[j] << "\n";
  }
  //
  outFile_ << "Initial Log Probs:\n";
  for (size_t i=0;i<filter_.logProbList_.size();i++){
    outFile_ << filter_.logProbList_[i]<< "\n";
  }
  //
  outFile_ << "Initial Model-Param Log Probs:\n";
  std::vector<double> mpProbsLog = modelUtils::calcModelParamProbLogWOExp(filter_.stateList_,filter_.logProbList_,modelParamPairs_);
  for (size_t i=0;i<mpProbsLog.size();i++){
    outFile_ << mpProbsLog[i]<< "\n";
  }
  //
  outFile_ << "Initial Model-Param Probs:\n";
  std::vector<double> mpProbs = modelUtils::calcModelParamProbLog(filter_.stateList_,filter_.logProbList_,modelParamPairs_);
  for (size_t i=0;i<mpProbs.size();i++){
    outFile_ << mpProbs[i]<< "\n";
  }
  //
  outFile_ << "Initial Pose in Rbt:\n";
  for (size_t i=0;i<poseInRbt_.size();i++){
    outFile_ << poseInRbt_[i]<< ",";
  }
  outFile_ << "\n";
  //
  outFile_ << "Total Steps:\n";
  outFile_ << stepsTotal_ << "\n";

}

void RealWorld::writeFileStepData(){
  //
  outFile_ << "Step:\n";
  outFile_ << step_ << "\n";
  //
  outFile_ << "Action Selection Type:\n";
  outFile_ << actionSelectionType_ << "\n";
  //
  outFile_ << "Action:\n";
  for (size_t i=0;i<action_.size();i++){
    outFile_ << action_[i]<< ",";
  }
  outFile_ << "\n";
  //
  outFile_ << "Pose in Rbt:\n";
  for (size_t i=0;i<poseInRbt_.size();i++){
    outFile_ << poseInRbt_[i]<< ",";
  }
  outFile_ << "\n";
  //
  outFile_ << "Observation in Obs:\n";
  for (size_t i=0;i<obs_.size();i++){
    outFile_ << obs_[i]<< ",";
  }
  outFile_ << "\n";
  //
  outFile_ << "Log Probs after Transition before Observation:\n";
  for (size_t i=0;i<filter_.logProbList_T_.size();i++){
    outFile_ << filter_.logProbList_T_[i]<< "\n";
  }
  //
  outFile_ << "Log Probs of Observation:\n";
  for (size_t i=0;i<filter_.logProbList_O_.size();i++){
    outFile_ << filter_.logProbList_O_[i]<< "\n";
  }
  //
  outFile_ << "Log Probs:\n";
  for (size_t i=0;i<filter_.logProbList_.size();i++){
    outFile_ << filter_.logProbList_[i]<< "\n";
  }
  //
  outFile_ << "Model-Param Log Probs:\n";
  std::vector<double> mpProbsLog = modelUtils::calcModelParamProbLogWOExp(filter_.stateList_,filter_.logProbList_,modelParamPairs_);
  for (size_t i=0;i<mpProbsLog.size();i++){
    outFile_ << mpProbsLog[i]<< "\n";
  }
  //
  outFile_ << "Model-Param Probs:\n";
  std::vector<double> mpProbs = modelUtils::calcModelParamProbLog(filter_.stateList_,filter_.logProbList_,modelParamPairs_);
  for (size_t i=0;i<mpProbs.size();i++){
    outFile_ << mpProbs[i]<< "\n";
  }
}


void printVect(std::vector<double> vect){
  for (size_t i=0; i<vect.size(); i++){
    std::cout << vect[i] << ",";
  }
  std::cout << std::endl;
}

double timeDiff(timespec& ts1, timespec& ts2){
  return (double) ts2.tv_sec - (double) ts1.tv_sec + ((double) ts2.tv_nsec - (double) ts1.tv_nsec)/1000000000; 
}

int main(int argc, char* argv[])
{
  if (argc > 6) { // We expect 3 arguments: the program name, the model number, the number of iterations, writeOutFile
    std::cerr << "Usage: " << argv[0] << "NUMBER OF ITERATIONS" << std::endl;
  }
  else {
    int steps;
    int modelNum;
    int writeOutFile;
    int actionSelectionType;
    int useRobot;
    if (argc == 1){
      modelNum = 0; // default: free model
      steps = 1; // default: run one step
      writeOutFile = 0; // default: don't write file
      actionSelectionType = 0; // default: go in order
      useRobot = 0; // default: use simulation
    }
    else if (argc == 2){
      modelNum = atoi(argv[1]);
      steps = 1; // default: run one step
      writeOutFile = 0; // default: don't write file
      actionSelectionType = 0; // default: go in order
      useRobot = 0; // default: use simulation
    }
    else if (argc == 3){
      modelNum = atoi(argv[1]);
      steps = atoi(argv[2]);
      writeOutFile = 0; // default: don't write file
      actionSelectionType = 0; // default: go in order
      useRobot = 0; // default: use simulation
    }
    else if (argc == 4){
      modelNum = atoi(argv[1]);
      steps = atoi(argv[2]);
      writeOutFile = atoi(argv[3]);
      actionSelectionType = 0; // default: go in order
      useRobot = 0; // default: use simulation
    }
    else if (argc == 5){
      modelNum = atoi(argv[1]);
      steps = atoi(argv[2]);
      writeOutFile = atoi(argv[3]);
      actionSelectionType = atoi(argv[4]);
      useRobot = 0; // default: use simulation
    }
    else{
      modelNum = atoi(argv[1]);
      steps = atoi(argv[2]);
      writeOutFile = atoi(argv[3]);
      actionSelectionType = atoi(argv[4]);
      useRobot = atoi(argv[5]);
    }


    std::cout << "steps: " << steps << std::endl;

    bool basic = true;
    if (basic){
      timespec ts1;
      timespec ts2;
      timespec ts3;
      
      clock_gettime(CLOCK_REALTIME, &ts1); // get time before constructor
      //std::cout << "hello" << std::endl;
      RealWorld world(modelNum,steps,writeOutFile,actionSelectionType,useRobot);
      //std::cout << "didnt get here" << std::endl;
      clock_gettime(CLOCK_REALTIME, &ts2); // get time after constructor

      //////////
      // DELETE - JUST FOR DEBUG

      for (size_t b =0;b<world.filter_.stateList_.size();b++){
	if (world.filter_.stateList_[b].model == 5){
	  stateStruct tempState = translator::stateTransition(world.filter_.stateList_[b],world.actionList_[4]);
	  std::cout << "Printing DEBUG for 3:" << std::endl;
	  std::cout << "Before state:" << std::endl;
	  std::cout << "Model: " << world.filter_.stateList_[b].model << std::endl;
	  std::cout << "Params: ";
	  for (size_t jj = 0; jj<world.filter_.stateList_[b].params.size(); jj++) {
	    std::cout << world.filter_.stateList_[b].params[jj] << ',';
	  }
	  std::cout << std::endl;
	  std::cout << "Vars: ";
	  for (size_t jj = 0; jj<world.filter_.stateList_[b].vars.size(); jj++) {
	    std::cout << world.filter_.stateList_[b].vars[jj] << ',';
	  }
	  std::cout << std::endl;
	  std::cout << "action:" << std::endl;
	  std::cout << world.actionList_[4][0] << "," << world.actionList_[4][0] << std::endl;
	  std::cout << "After state:" << std::endl;
	  std::cout << "Model: " << tempState.model << std::endl;
	  std::cout << "Params: ";
	  for (size_t jj = 0; jj<tempState.params.size(); jj++) {
	    std::cout << tempState.params[jj] << ',';
	  }
	  std::cout << std::endl;
	  std::cout << "Vars: ";
	  for (size_t jj = 0; jj<tempState.vars.size(); jj++) {
	    std::cout << tempState.vars[jj] << ',';
	  }
	  std::cout << std::endl;
	}
      }

      ////////////

      world.runWorld(steps);
      
      clock_gettime(CLOCK_REALTIME, &ts3); // get time after running world

      world.outFile_ << "Time for constructor:\n" << timeDiff(ts1,ts2) << "\n";
      world.outFile_ << "Time for running world:\n" << timeDiff(ts2,ts3) << "\n";
      world.outFile_ << "Time per step:\n" << timeDiff(ts2,ts3)/steps << "\n";

      std::cout << "Time for constructor:\n" << timeDiff(ts1,ts2) << std::endl;
      std::cout << "Time for running world:\n" << timeDiff(ts2,ts3) << std::endl;
      std::cout << "Time per step:\n" << timeDiff(ts2,ts3)/steps << std::endl;
    }
    else{
      int numExps = 10;
      double conf = 0.7;
      std::vector<int> stepsNeededVect;
      for (size_t i=0;i<numExps;i++){
	RealWorld world(modelNum,steps,writeOutFile,actionSelectionType,useRobot);
	stepsNeededVect.push_back(world.runWorldToConf(steps,conf));
      }
      std::cout << "Steps Needed:" << std::endl;
      for (size_t i=0;i<stepsNeededVect.size();i++){
	std::cout << stepsNeededVect[i] << ",";
      }
      std::cout << std::endl << "Avg:" << std::endl;
      std::cout << std::accumulate(stepsNeededVect.begin(),stepsNeededVect.end(),0.0)/numExps << std::endl;
    }
    //std::cout << RELATIVE << std::endl;
    //std::cout << WORKSPACE[0][0] << std::endl;
    //std::cout << WORKSPACE[0][1] << std::endl;
    //std::cout << WORKSPACE[1][0] << std::endl;
    //std::cout << WORKSPACE[1][1] << std::endl;
  }
  return 1;
}
