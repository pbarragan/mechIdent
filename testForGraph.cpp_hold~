#include <iostream>
#include "stateStruct.h"
#include "translator.h"
#include <vector>

void printState(stateStruct state){
  std::cout << "model: " << state.model << std::endl;
  std::cout << "params: ";
  for (size_t i=0;i<state.params.size();i++){
    std::cout << state.params[i] << ",";
  }
  std::cout << std::endl;
  std::cout << "vars: ";
  for (size_t i=0;i<state.vars.size();i++){
    std::cout << state.vars[i] << ",";
  }
  std::cout << std::endl;  
}

int main(){
  std::vector<double> action1;
  std::vector<double> action2;
  std::vector<double> action3;

  action1.push_back(0.12);
  action1.push_back(-0.12);
  action2.push_back(0.12);
  action2.push_back(0.12);
  action3.push_back(-0.12);
  action3.push_back(0.12);

  // create free state
  stateStruct freeState;
  {
    freeState.model = 0;
    std::vector<double> stateVars;
    stateVars.push_back(0.0);
    stateVars.push_back(0.0);
    freeState.vars = stateVars;
  }
  // create rev state
  stateStruct revState;
  {
    revState.model = 2;
    std::vector<double> stateParams;
    stateParams.push_back(0.3);
    stateParams.push_back(0.3);
    stateParams.push_back(0.42426);
    revState.params = stateParams;
    std::vector<double> stateVars;
    stateVars.push_back(-2.356);
    revState.vars = stateVars;
  }
  // create prisPris state
  stateStruct prisPrisState;
  {
    prisPrisState.model = 5;
    std::vector<double> stateParams;
    stateParams.push_back(-0.1);
    stateParams.push_back(-0.1);
    stateParams.push_back(0.0);
    stateParams.push_back(0.1);
    stateParams.push_back(0.1);
    prisPrisState.params = stateParams;
    std::vector<double> stateVars;
    stateVars.push_back(0.1);
    stateVars.push_back(0.1);
    prisPrisState.vars = stateVars;
  }

  std::cout << "Free, 1:" << std::endl;
  printState(translator::stateTransition(freeState,action1));
  std::cout << "Rev, 1:" << std::endl;
  printState(translator::stateTransition(revState,action1));
  std::cout << "PrisPris, 1:" << std::endl;
  printState(translator::stateTransition(prisPrisState,action1));
  std::cout << "Free, 2:" << std::endl;
  printState(translator::stateTransition(freeState,action2));
  std::cout << "Rev, 2:" << std::endl;
  printState(translator::stateTransition(revState,action2));
  std::cout << "PrisPris, 2:" << std::endl;
  printState(translator::stateTransition(prisPrisState,action2));
  std::cout << "Free, 3:" << std::endl;
  printState(translator::stateTransition(freeState,action3));
  std::cout << "Rev, 3:" << std::endl;
  printState(translator::stateTransition(revState,action3));
  std::cout << "PrisPris, 3:" << std::endl;
  printState(translator::stateTransition(prisPrisState,action3));
  return 1;
}
