#include <iostream>

#include "mechanism.h"
#include "mechFree.h"
#include "mechFixed.h"
#include "mechRev.h"
#include "mechPris.h"
#include "mechRevPrisL.h"
#include "mechPrisPrisL.h"

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

  std::vector< std::vector<double> > workspace (2,std::vector<double> (2,0.0));
  workspace[0][0] = -0.2;
  workspace[0][1] = 0.2;
  workspace[1][0] = -0.2;
  workspace[1][1] = 0.2;

  MechFree* free = new MechFree();
  stateStruct startState;
  startState.model = 0;
  std::vector<double> tempVars (2,0.0);
  startState.vars = tempVars;
  free->initialize(startState);
  std::vector<double> action (2,-0.15);
  stateStruct endState = free->simulate(action);
  stateStruct freeState = startState;

  std::cout << "Model: " << endState.model << std::endl;
  std::cout << "Params size: " << endState.params.size() << std::endl;
  std::cout << "Vars size: " << endState.vars.size() << std::endl;
  std::cout << "Vars: " << endState.vars[0] << ", " << endState.vars[1] << std::endl;
  delete free;

  MechFixed* fixed = new MechFixed();
  stateStruct startState2;
  startState2.model = 1;
  std::vector<double> tempParams2 (2,0.20);
  startState2.params = tempParams2;
  fixed->initialize(startState2);
  std::vector<double> action2 (2,0.2);
  stateStruct endState2 = fixed->simulate(action2);
  stateStruct fixedState = startState2;
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
  revState.params.push_back(0.1);
  revState.vars.push_back(0.785);
  
  std::vector<double> revAction;
  revAction.push_back(0.854);
  revAction.push_back(0.854);

  stateStruct nextRevState = rev->initAndSim(revState,revAction);

  std::cout << "rev:\nmodel: " << nextRevState.model << std::endl;
  std::cout << "params: " << nextRevState.params.size() << std::endl;
  printVect(nextRevState.params);
  std::cout << "vars: " << nextRevState.vars.size() << std::endl;
  printVect(nextRevState.vars);

  std::vector<double> obsRev = rev->stToObs(nextRevState);
  std::cout << "obs:" << std::endl;
  for (size_t i=0;i<obsRev.size();i++){
    std::cout << obsRev[i] << ",";
  }
  std::cout << std::endl;

  std::vector<double> rbtRev = rev->stToRbt(nextRevState);
  std::cout << "rbt:" << std::endl;
  for (size_t i=0;i<rbtRev.size();i++){
    std::cout << rbtRev[i] << ",";
  }
  std::cout << std::endl;
  delete rev;

  // Primsmatic
  MechPris* pris = new MechPris();

  stateStruct prisState;
  prisState.model=3;
  prisState.params.push_back(0.1);
  prisState.params.push_back(0.0);
  prisState.params.push_back(0.7856);
  prisState.vars.push_back(0.0);
  
  std::vector<double> prisAction;
  prisAction.push_back(0.15);
  prisAction.push_back(0.05);

  stateStruct nextPrisState = pris->initAndSim(prisState,prisAction);

  std::cout << "pris:\nmodel: " << nextPrisState.model << std::endl;
  std::cout << "params: " << nextPrisState.params.size() << std::endl;
  printVect(nextPrisState.params);
  std::cout << "vars: " << nextPrisState.vars.size() << std::endl;
  printVect(nextPrisState.vars);

  std::vector<double> obsPris = pris->stToObs(nextPrisState);
  std::cout << "obs:" << std::endl;
  for (size_t i=0;i<obsPris.size();i++){
    std::cout << obsPris[i] << ",";
  }
  std::cout << std::endl;

  std::vector<double> rbtPris = pris->stToRbt(nextPrisState);
  std::cout << "rbt:" << std::endl;
  for (size_t i=0;i<rbtPris.size();i++){
    std::cout << rbtPris[i] << ",";
  }
  std::cout << std::endl;
  delete pris;

  // RevPris Latch
  MechRevPrisL* revPrisL = new MechRevPrisL();

  stateStruct revPrisLState;
  revPrisLState.model=4;
  revPrisLState.params.push_back(-0.20);
  revPrisLState.params.push_back(0.0);
  revPrisLState.params.push_back(0.1);
  revPrisLState.params.push_back(0.0);
  revPrisLState.params.push_back(0.10);

  revPrisLState.vars.push_back(0.0);
  revPrisLState.vars.push_back(0.0);

  std::vector<double> revPrisLAction;
  revPrisLAction.push_back(0.15);
  revPrisLAction.push_back(0.15);

  //std::cout << "Is revPrisLState valid: " << revPrisL->isStateValid(revPrisLState,workspace) << std::endl;
  
  //delete revPrisL;

  //revPrisL = new MechRevPrisL();

  stateStruct nextRevPrisLState = revPrisL->initAndSim(revPrisLState,revPrisLAction);

  std::cout << "revPrisL:\nmodel: " << nextRevPrisLState.model << std::endl;
  std::cout << "params: " << nextRevPrisLState.params.size() << std::endl;
  printVect(nextRevPrisLState.params);
  std::cout << "vars: " << nextRevPrisLState.vars.size() << std::endl;
  printVect(nextRevPrisLState.vars);

  std::vector<double> obsRevPrisL = revPrisL->stToObs(nextRevPrisLState);
  std::cout << "obs:" << std::endl;
  for (size_t i=0;i<obsRevPrisL.size();i++){
    std::cout << obsRevPrisL[i] << ",";
  }
  std::cout << std::endl;

  std::vector<double> rbtRevPrisL = revPrisL->stToRbt(nextRevPrisLState);
  std::cout << "rbt:" << std::endl;
  for (size_t i=0;i<rbtRevPrisL.size();i++){
    std::cout << rbtRevPrisL[i] << ",";
  }
  std::cout << std::endl;
  delete revPrisL;

  // PrisPris Latch
  MechPrisPrisL* prisPrisL = new MechPrisPrisL();

  stateStruct prisPrisLState;
  prisPrisLState.model=5;
  prisPrisLState.params.push_back(-0.10);
  prisPrisLState.params.push_back(-0.10);
  prisPrisLState.params.push_back(0.0);
  prisPrisLState.params.push_back(0.1);
  prisPrisLState.params.push_back(0.1);

  prisPrisLState.vars.push_back(0.1);
  prisPrisLState.vars.push_back(0.1);

  std::vector<double> prisPrisLAction;
  prisPrisLAction.push_back(-0.15);
  prisPrisLAction.push_back(-0.15);

  std::cout << "Is prisPrisLState valid: " << prisPrisL->isStateValid(prisPrisLState,workspace) << std::endl;
  
  delete prisPrisL;

  prisPrisL = new MechPrisPrisL();

  stateStruct nextPrisPrisLState = prisPrisL->initAndSim(prisPrisLState,prisPrisLAction);

  std::cout << "prisPrisL:\nmodel: " << nextPrisPrisLState.model << std::endl;
  std::cout << "params: " << nextPrisPrisLState.params.size() << std::endl;
  printVect(nextPrisPrisLState.params);
  std::cout << "vars: " << nextPrisPrisLState.vars.size() << std::endl;
  printVect(nextPrisPrisLState.vars);

  std::vector<double> obsPrisPrisL = prisPrisL->stToObs(nextPrisPrisLState);
  std::cout << "obs:" << std::endl;
  for (size_t i=0;i<obsPrisPrisL.size();i++){
    std::cout << obsPrisPrisL[i] << ",";
  }
  std::cout << std::endl;

  std::vector<double> rbtPrisPrisL = prisPrisL->stToRbt(nextPrisPrisLState);
  std::cout << "rbt:" << std::endl;
  for (size_t i=0;i<rbtPrisPrisL.size();i++){
    std::cout << rbtPrisPrisL[i] << ",";
  }
  std::cout << std::endl;
  delete prisPrisL;

  // check validity through translator
  std::cout << "Translator valid: " << translator::isStateValid(prisPrisLState,workspace) << std::endl;


  std::vector<double> fake1 (2,0.0);
  std::vector<double> fake2 (2,0.0);
  fake1[1] = 0.000000001;
  std::cout << "check: " << (fake1==fake2) << std::endl;

  // logUtilities shit
  
  /*
  stateStruct testState4;
  testState4.model = 4;
  std::vector<double> comeOn;
  comeOn.push_back(-0.2);
  comeOn.push_back(0.0);
  comeOn.push_back(0.1);
  comeOn.push_back(0.0);
  comeOn.push_back(0.1);

  

  testState4.params
  */

  return 1;
}
