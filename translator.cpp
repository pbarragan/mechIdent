#include "translator.h"

//include mechanisms
#include "mechanisms/mechFree.h"
#include "mechanisms/mechFixed.h"
#include "mechanisms/mechRev.h"
#include "mechanisms/mechPris.h"
#include "mechanisms/mechRevPrisL.h"
#include "mechanisms/mechPrisPrisL.h"

#include <iostream> // DELETE

enum MechTypes
  {
    MECH_FREE,
    MECH_FIXED,
    MECH_REV,
    MECH_PRIS,
    MECH_REV_PRIS_L,
    MECH_PRIS_PRIS_L
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
  case MECH_REV:
    mechPtr = new MechRev();
    break;
  case MECH_PRIS:
    mechPtr = new MechPris();
    break;
  case MECH_REV_PRIS_L:
    mechPtr = new MechRevPrisL();
    break;
  case MECH_PRIS_PRIS_L:
    mechPtr = new MechPrisPrisL();
    break;
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
  // DELETE
  /*
  if (nextState.model == 4){
    MechRevPrisL mech;
    std::vector<double> hold = mech.stToRbt(nextState);
    std::cout << "model 4 r:" << nextState.params[2] << std::endl;
    std::cout << "model 4 d start:" << state.vars[1] << std::endl;
    std::cout << "model 4 d next:" << nextState.vars[1] << std::endl;
    std::cout << "action:" << action[0] << "," << action[1] << std::endl;
    std::cout << "model 4 in rbt:" << std::endl;
    std::cout << hold[0] << std::endl;
    std::cout << hold[1] << std::endl;
  }
  */
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

bool translator::isStateValid(stateStruct& state,std::vector< std::vector<double> >& workspace){
  Mechanism* mechPtr = createMechanism(state.model);
  bool valid = mechPtr->isStateValid(state,workspace);
  delete mechPtr;
  return valid;
}
