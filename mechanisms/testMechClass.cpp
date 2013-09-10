#include <iostream>

#include "mechanism.h"
#include "mechFree.h"
#include "mechFixed.h"
#include "mechRev.h"

#include "../translator.h"
#include "../setupUtils.h"
#include "../logUtils.h"

//#include "../stateStruct.h"

void printVect(std::vector<double> vect){
  for (size_t i=0; i<vect.size(); i++){
    std::cout << vect[i] << ",";
  }
  std::cout << std::endl;
}

int main(){
  MechFree* free = new MechFree();
  stateStruct startState;
  startState.model = 0;
  std::vector<double> tempVars (2,0.0);
  startState.vars = tempVars;
  free->initialize(startState);
  std::vector<double> action (2,-0.15);
  stateStruct endState = free->simulate(action);
  
  std::cout << "Model: " << endState.model << std::endl;
  std::cout << "Params size: " << endState.params.size() << std::endl;
  std::cout << "Vars size: " << endState.vars.size() << std::endl;
  std::cout << "Vars: " << endState.vars[0] << ", " << endState.vars[1] << std::endl;
  delete free;

  MechFixed* fixed = new MechFixed();
  stateStruct startState2;
  startState2.model = 1;
  std::vector<double> tempParams2 (2,0.2);
  startState2.params = tempParams2;
  fixed->initialize(startState2);
  std::vector<double> action2 (2,0.2);
  stateStruct endState2 = fixed->simulate(action2);
  
  std::cout << "Model: " << endState2.model << std::endl;
  std::cout << "Params size: " << endState2.params.size() << std::endl;
  std::cout << "Vars size: " << endState2.vars.size() << std::endl;
  std::cout << "Params: " << endState2.params[0] << ", " << endState2.params[1] << std::endl;
  delete fixed;

  MechFree* free2 = new MechFree();
  MechFixed* fixed2 = new MechFixed();
  stateStruct endState3 = free2->initAndSim(startState,action);
  stateStruct endState4 = fixed2->initAndSim(startState2,action2);

  std::cout << "Model: " << endState3.model << std::endl;
  std::cout << "Params size: " << endState3.params.size() << std::endl;
  std::cout << "Vars size: " << endState3.vars.size() << std::endl;
  std::cout << "Vars: " << endState3.vars[0] << ", " << endState3.vars[1] << std::endl;
  
  std::cout << "Model: " << endState4.model << std::endl;
  std::cout << "Params size: " << endState4.params.size() << std::endl;
  std::cout << "Vars size: " << endState4.vars.size() << std::endl;
  std::cout << "Params: " << endState4.params[0] << ", " << endState4.params[1] << std::endl;

  printVect(free2->stToObs(startState));
  printVect(free2->stToRbt(startState));
  printVect(free2->sensToObs(tempVars));
  printVect(free2->rbtToObs(tempVars));

  printVect(fixed2->stToObs(startState2));
  printVect(fixed2->stToRbt(startState2));
  printVect(fixed2->sensToObs(tempVars));
  printVect(fixed2->rbtToObs(tempVars));

  delete free2;
  delete fixed2;

  stateStruct endState5 = translator::stateTransition(startState,action);
  stateStruct endState6 = translator::stateTransition(startState2,action2);

  std::cout << "Model: " << endState5.model << std::endl;
  std::cout << "Params size: " << endState5.params.size() << std::endl;
  std::cout << "Vars size: " << endState5.vars.size() << std::endl;
  std::cout << "Vars: " << endState5.vars[0] << ", " << endState5.vars[1] << std::endl;
  
  std::cout << "Model: " << endState6.model << std::endl;
  std::cout << "Params size: " << endState6.params.size() << std::endl;
  std::cout << "Vars size: " << endState6.vars.size() << std::endl;
  std::cout << "Params: " << endState6.params[0] << ", " << endState6.params[1] << std::endl;

  printVect(translator::translateStToObs(startState));
  printVect(translator::translateStToObs(startState2));
  printVect(translator::translateStToRbt(startState));
  printVect(translator::translateStToRbt(startState2));
  printVect(translator::translateSensToObs(tempVars));
  printVect(translator::translateSensToObs(tempParams2));

  std::cout << "testing setupUtils:" << std::endl;
  std::vector<stateStruct> stateList;
  std::vector<stateStruct> modelParamPairs;
  std::vector<double> probList;
  std::vector< std::vector<double> > actionList;

  setupUtils::setupStates(stateList,modelParamPairs);
  setupUtils::setupUniformPrior(stateList,probList,modelParamPairs);
  setupUtils::setupActions(actionList);

  std::cout << "stateList: " << std::endl;
  for (size_t i=0;i<stateList.size();i++){
    std::cout << stateList[i].model << std::endl;
    printVect(stateList[i].params);
    printVect(stateList[i].vars);
  }  
  std::cout << "modelParamPairs: " << std::endl;
  for (size_t i=0;i<modelParamPairs.size();i++){
    std::cout << modelParamPairs[i].model << std::endl;
    printVect(modelParamPairs[i].params);
  }
  std::cout << "probList: " << std::endl;
  printVect(probList);
  for (size_t i = 0;i<probList.size();i++){
    std::cout << logUtils::safe_exp(probList[i]) << std::endl;
  }
  std::cout << "actionList: " << std::endl;
  for (size_t i=0;i<actionList.size();i++){
    printVect(actionList[i]);
  }

  // quick tests
  std::vector< std::vector<double> > testVect;
  std::vector<double> emptyVect;
  testVect.push_back(emptyVect);
  std::cout << testVect.size() << std::endl;

  // Revolute
  MechRev* rev = new MechRev();

  stateStruct revState;
  revState.model=2;
  revState.params.push_back(0.1);
  revState.params.push_back(0.1);
  revState.params.push_back(0.5);
  revState.vars.push_back(0.0);
  
  std::vector<double> revAction;
  revAction.push_back(0.854);
  revAction.push_back(0.854);

  stateStruct nextRevState = rev->initAndSim(revState,revAction);

  std::cout << "rev:\nmodel: " << nextRevState.model << std::endl;
  std::cout << "params: " << nextRevState.params.size() << std::endl;
  printVect(nextRevState.params);
  std::cout << "vars: " << nextRevState.vars.size() << std::endl;
  printVect(nextRevState.vars);
  delete rev;

  return 1;
}
