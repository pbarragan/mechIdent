// MechFixed sub class

#include "mechFixed.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream> // DELETE

////////////////////////////////////////////////////////////////////////////////
//                             Redefined Section                              //
////////////////////////////////////////////////////////////////////////////////

// virtual
void MechFixed::initialize(stateStruct& startState){
  // Only initialize start pose for fixed mechanism to get it back
  setStartWithState(startState);
}

// virtual 
void MechFixed::exitPhysics(){
  // No cleanup needed for fixed mechanism
}

////////////////////////////////////////////////////////////////////////////////
//                           End Redefined Section                            //
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//                         Sub (Derived) Class Section                        //
////////////////////////////////////////////////////////////////////////////////

// virtual 
MechFixed::~MechFixed(){
  // Nothing needs to happen here
  // This should then call Mechanism::~Mechanism() automatically
}

// Not Defined in super (base) class

// virtual 
void MechFixed::setStartWithState(stateStruct& startState){
  // State looks like:
  // Model: 1
  // Params: x,y in rbt space
  // Vars:
  
  // Set start pose so you can extract it in MechFixed::returnStateOfWorld()
  // This is a hack to maintain abstraction
  // Create an x,y,z vector in rbt
  std::vector<double> tempVarsRbt = startState.params;
  tempVarsRbt.push_back(1.0); // set z_rbt to 1.0
  
  // Convert to sim and set
  std::vector<double> tempVarsSim = convCoordsRbtToSim(tempVarsRbt);
  startPose_ = convStdVectToBtVect3(tempVarsSim);
}

/*
// used to be virtual 
void MechFixed::setGoalWithAction(std::vector<double>& action){
  // Action looks like:
  // x,y in rbt space

  // No need to set goal
}
*/

// virtual 
stateStruct MechFixed::returnStateOfWorld(){
  // State looks like:
  // Model: 1
  // Params: x,y in rbt space
  // Vars: 

  stateStruct endState;
  endState.model = 1;
  //only have set startPose_. pose_ is still not set.
  std::vector<double> tempVarsSim = convBtVect3ToStdVect(startPose_);
  endState.params = convCoordsSimToRbt(tempVarsSim);
  endState.params.pop_back();
  return endState;
}

// virtual 
std::vector<double> MechFixed::stToObs(stateStruct& state){
  // State looks like:
  // Model: 1
  // Params: x,y in rbt space
  // Vars: 

  // Observation looks like:
  // x,y in rbt space
  return state.params;
}

// virtual 
std::vector<double> MechFixed::stToRbt(stateStruct& state){
  // State looks like:
  // Model: 1
  // Params: x,y in rbt space
  // Vars: 

  // Robot looks like:
  // x,y in rbt space
  return state.params;
}

bool MechFixed::isStateValid(stateStruct& state,std::vector< std::vector<double> >& workspace){
  // State looks like:
  // Model: 1
  // Params: x,y in rbt space
  // Vars: 
  
  // Single conditions to check
  // Check if state places rbt outside of rbt workspace
  std::vector<double> rbt = stToRbt(state);
  if (rbt[0]<workspace[0][0] || rbt[0]>workspace[0][1] || rbt[1]<workspace[1][0] || rbt[1]>workspace[1][1]){ /*std::cout << "error 2" << std::endl;*/ return false;}
  else return true;
}

////////////////////////////////////////////////////////////////////////////////
//                       End Sub (Derived) Class Section                      //
////////////////////////////////////////////////////////////////////////////////
