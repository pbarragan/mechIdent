#include "translator.h"

//include mechanisms
#include "mechanisms/mechFree.h"
#include "mechanisms/mechFixed.h"
//#include "mechanisms/mechRev.h"
//#include "mechanisms/mechPris.h"
//#include "mechanisms/mechRevPris.h"

enum MechTypes
  {
    MECH_FREE,
    MECH_FIXED/*,
    MECH_REV,
    MECH_PRIS,
    MEHC_REVPRIS*/
  };

Mechanism* translator::createMechanism(int choice){
  Mechanism* mechPtr;
  switch(choice){
  case MECH_FREE:
    mechPtr = new MechFree();
    break;
  case MECH_FIXED:
    mechPtr = new MechFixed();
    break;
    /*
  case MECH_REV:
    mechPtr = new MechRev();
    break;
  case MECH_PRIS:
    mechPtr = new MechPris();
    break;
  case MECH_REVPRIS:
    mechPtr = new MechRevPris();
    break;*/
  }
  return mechPtr;
}

// overloaded
stateStruct translator::stateTransition(stateStruct& state, std::vector<double>& action){
  Mechanism* mechPtr = createMechanism(state.model);
  stateStruct nextState = mechPtr->initAndSim(state,action);
  delete mechPtr;
  return nextState;
}

// overloaded
stateStruct translator::stateTransition(stateStruct& state, std::vector<double>& action, sasUtils::mapPairSVS& sasList){
  stateStruct nextState = sasUtils::getFromSAS(sasList,state,action);
  return nextState;
}

std::vector<double> translator::translateStToObs(stateStruct& state){
  Mechanism* mechPtr = createMechanism(state.model);
  std::vector<double> stateInObs = mechPtr->stToObs(state);
  delete mechPtr;
  return stateInObs;
}

std::vector<double> translator::translateStToRbt(stateStruct& state){
  Mechanism* mechPtr = createMechanism(state.model);
  std::vector<double> stateInRbt = mechPtr->stToRbt(state);
  delete mechPtr;
  return stateInRbt;
}

std::vector<double> translator::translateSensToObs(std::vector<double>& obs){
  Mechanism* mechPtr = new Mechanism();
  std::vector<double> sensInObs = mechPtr->sensToObs(obs);
  delete mechPtr;
  return sensInObs;
}

