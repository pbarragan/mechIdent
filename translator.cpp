//translator
#include "translator.h"

//include mechanisms
#include "mechanisms/mechFree.h"
#include "mechanisms/mechFixed.h"
#include "mechanisms/mechRev.h"
#include "mechanisms/mechPris.h"
#include "mechanisms/mechRevPris.h"

enum MechTypes
  {
    MECH_FREE,
    MECH_FIXED,
    MECH_REV,
    MECH_PRIS,
    MEHC_REVPRIS
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
  case MECH_REVPRIS:
    mechPtr = new MechRevPris();
    break;
  }
  return mechPtr;
}

std::vector<double> translator::stateTransition(std::vector<double> state, std::vector<double> action){
  Mechanism* mechPtr = createMechanism((int) state[0]);
  std::vector<double> nextState = mechPtr->simulate(state,action);
  delete mechPtr;
  return nextState;
}

std::vector<double> translator::translateStateToO(std::vector<double> state){
  Mechanism* mechPtr = createMechanism((int) state[0]);
  std::vector<double> stateInO =mechPtr->StoO(state);
  delete mechPtr;
  return stateInO;
}

std::vector<double> translator::translateObsToO(std::vector<double> obs){
  //this doesn't need to happen like this
  Mechanism* mechPtr;
  mechPtr = new MechObs();
  std::vector<double> obsInO =mechPtr->ObstoO(obs);
  delete mechPtr;
  return obsInO;
}

