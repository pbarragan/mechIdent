// MechFree sub class

#include "mechFree.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream> // DELETE

////////////////////////////////////////////////////////////////////////////////
//                             Redefined Section                              //
////////////////////////////////////////////////////////////////////////////////

// virtual
void MechFree::initialize(stateStruct& startState){
  // Initialize standard physics for Mechanism
  Mechanism::initialize(startState);

  // Initialize additional physics for MechFree
  // REMEMBER TO SET pose_ to startPose_ and run Mechanism::updatePose()??

  //----------------OBJECT CREATION SECTION-------------------//
  //Calculate the required origins based on startPose_ given by the runSimulation function
  btVector3 link0O = startPose_; // link0 origin
  btVector3 rbtO = startPose_; // rbt origin
  
  //----------make link0 rigid body (the sliding box)---------//
  // shape
  const btVector3 link0BoxHalfExtents( 1.0f, 1.0f, 1.0f );
  link0CS_ = new btBoxShape(link0BoxHalfExtents);
  // position
  btTransform link0BT;
  link0BT.setIdentity();
  link0BT.setOrigin(link0O);
  // mass
  btScalar link0Mass = 1.0f;
  //create the link0 rigid body
  link0RB_ = createRigidBody(link0CS_,link0Mass,link0BT);
  //add it to the dynamics world
  dynamicsWorld_->addRigidBody(link0RB_);
  //--------end make link0 rigid body (the sliding box)-------//

  //--------make rbt rigid body (the robots manipulator)------//
  // shape
  const btVector3 rbtBoxHalfExtents( 0.1f, 0.5f, 0.1f );
  rbtCS_ = new btBoxShape(rbtBoxHalfExtents);
  // position
  rbtBT_.setIdentity();
  rbtBT_.setOrigin(rbtO);
  // mass
  btScalar rbtMass = 0.1f;
  //create the rbt rigid body
  rbtRB_ = createRigidBody(rbtCS_,rbtMass,rbtBT_);
  //add it to the dynamics world
  dynamicsWorld_->addRigidBody(rbtRB_);
  //------end make rbt rigid body (the robots manipulator)----//

  //--------------END OBJECT CREATION SECTION-----------------//

  //--------------CONSTRAINT CREATION SECTION-----------------//
  
  // create hinge constraint between rbt and link0
  const btVector3 pivotInA( 0.0f, 0.0f, 0.0f );   
  const btVector3 pivotInB( 0.0f, 0.0f, 0.0f );
  btVector3 axisInA( 0.0f, 1.0f, 0.0f );
  btVector3 axisInB( 0.0f, 1.0f, 0.0f );
  bool useReferenceFrameA = false;
  link0RbtHC_ = new btHingeConstraint(*link0RB_,*rbtRB_,pivotInA,pivotInB,axisInA,axisInB,useReferenceFrameA);
  // add constraint to the world
  const bool isDisableCollisionsBetweenLinkedBodies = true; //disable collisions
  dynamicsWorld_->addConstraint(link0RbtHC_,isDisableCollisionsBetweenLinkedBodies);
  
  //--------------END CONSTRAINT CREATION SECTION-----------------//

  //---------INITIALIZE REMAINING PARAMETERS SECTION----------//

  // Disable deactivation for every object
  link0RB_->forceActivationState(DISABLE_DEACTIVATION);
  rbtRB_->forceActivationState(DISABLE_DEACTIVATION);
  link0RB_->setSleepingThresholds(0.0,0.0);
  rbtRB_->setSleepingThresholds(0.0,0.0);

  // change friction
  rbtRB_->setFriction(0.1);
  link0RB_->setFriction(0.1);

  // ALWAYS call updatePose() before starting to initiliaze pose_ and rbtBT_
  updatePose();

  //-------END INITIALIZE REMAINING PARAMETERS SECTION-------//
}

// virtual
void MechFree::exitPhysics(){
  // cleanup in the reverse order of creation/initialization
  // remove additional things created in this subclass
  delete link0RbtHC_;
  delete link0CS_;
}

////////////////////////////////////////////////////////////////////////////////
//                           End Redefined Section                            //
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//                         Sub (Derived) Class Section                        //
////////////////////////////////////////////////////////////////////////////////

// virtual
MechFree::~MechFree(){
  exitPhysics();
  // This should then call Mechanism::~Mechanism() automatically
}

// Not Defined in super (base) class
// virtual
void MechFree::setStartWithState(stateStruct& startState){
  // State looks like:
  // Model: 0
  // Params:
  // Vars: x,y in rbt space
  
  // Create an x,y,z vector in rbt
  std::vector<double> tempVarsRbt = startState.vars;
  tempVarsRbt.push_back(1.0); // set z_rbt to 1.0
  
  // Convert to sim and set
  std::vector<double> tempVarsSim = convCoordsRbtToSim(tempVarsRbt);
  startPose_ = convStdVectToBtVect3(tempVarsSim);
}

/*
// used to be virtual
void MechFree::setGoalWithAction(std::vector<double>& action){
  // Action looks like:
  // x,y in rbt space

  // Create an x,y,z vector in rbt
  std::vector<double> tempActRbt = action;
  tempActRbt.push_back(1.0); // set z_rbt to 1.0

  // Convert to sim and set
  std::vector<double> tempActSim = convCoordsRbtToSim(tempActRbt);
  goalPose_ = convStdVectToBtVect3(tempActSim);
}
*/

// virtual
stateStruct MechFree::returnStateOfWorld(){
  // State looks like:
  // Model: 0
  // Params:
  // Vars: x,y in rbt space

  stateStruct endState;
  endState.model = 0;
  std::vector<double> tempVarsSim = convBtVect3ToStdVect(pose_);
  endState.vars = convCoordsSimToRbt(tempVarsSim);
  endState.vars.pop_back();
  return endState;
}

// virtual
std::vector<double> MechFree::stToObs(stateStruct& state){
  // State looks like:
  // Model: 0
  // Params:
  // Vars: x,y in rbt space

  // Observation looks like:
  // x,y in rbt space
  return state.vars;
}

// virtual
std::vector<double> MechFree::stToRbt(stateStruct& state){
  // State looks like:
  // Model: 0
  // Params:
  // Vars: x,y in rbt space

  // Robot looks like:
  // x,y in rbt space
  return state.vars;
}

bool MechFree::isStateValid(stateStruct& state,std::vector< std::vector<double> >& workspace){
  // State looks like:
  // Model: 0
  // Params:
  // Vars: x,y in rbt space
  
  // Single conditions to check
  // Check if state places rbt outside of rbt workspace
  std::vector<double> rbt = stToRbt(state);
  if (rbt[0]<workspace[0][0] || rbt[0]>workspace[0][1] || rbt[1]<workspace[1][0] || rbt[1]>workspace[1][1]){ /*std::cout << "error 2" << std::endl;*/ return false;}
  else return true;
}

////////////////////////////////////////////////////////////////////////////////
//                       End Sub (Derived) Class Section                      //
////////////////////////////////////////////////////////////////////////////////
