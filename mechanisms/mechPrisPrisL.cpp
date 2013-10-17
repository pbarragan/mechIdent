// MechPrisPrisL sub class

#include "mechPrisPrisL.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream> // DELETE

////////////////////////////////////////////////////////////////////////////////
//                             Redefined Section                              //
////////////////////////////////////////////////////////////////////////////////

// virtual
void MechPrisPrisL::initialize(stateStruct& startState){
  // Set the size scale for objects in this world
  sizeScale_ = 0.01;
  // Set offsets before running Mechanism::initialize()
  o3_ = 0.1*sizeScale_; // tiny spacing between latch and handle
  l1_ = 6*sizeScale_; // length of side pieces of latch

  // Initialize standard physics for Mechanism
  Mechanism::initialize(startState);

  // Initialize additional physics for MechPrisPrisL
  // REMEMBER TO SET pose_ to startPose_ and run Mechanism::updatePose()??

  // Get rid of gravity even though it was set in Mechanism::initialize
  dynamicsWorld_->setGravity(btVector3(0,0,0));

  //----------------OBJECT CREATION SECTION-------------------//
  // Use start poses calculated by 
  btVector3 fxdO = startPoseFxd_; // fxd origin
  btVector3 link1O = startPoseLink1_; // link1 origin
  btVector3 link0O = startPoseLink0_; // link0 origin
  btVector3 rbtO = startPose_; // rbt origin

  btVector3 fxdAO = startPoseFxdA_; // fxd origin
  btVector3 fxdBO = startPoseFxdB_; // fxd origin
  btVector3 fxdCO = startPoseFxdC_; // fxd origin

  //----------make fxd rigid body (the pivot)---------//
  // shape
  const btVector3 fxdBoxHalfExtents( sizeScale_, 1.0f, sizeScale_ );
  fxdCS_ = new btBoxShape(fxdBoxHalfExtents);
  // position
  btTransform fxdBT;
  fxdBT.setIdentity();
  fxdBT.setOrigin(fxdO);
  fxdBT.setRotation(startQuatLink1_);
  // mass
  btScalar fxdMass = 0.0f; // fixed
  // create the fxd rigid body
  fxdRB_ = createRigidBody(fxdCS_,fxdMass,fxdBT);
  // add it to the dynamics world
  dynamicsWorld_->addRigidBody(fxdRB_);
  //--------end make fxd rigid body (the pivot)-------//

  //----------make link1 rigid body (the sliding arm near the pivot)---------//
  // shape
  const btVector3 link1BoxHalfExtents( link1Len_/2, sizeScale_, sizeScale_ );
  link1CS_ = new btBoxShape(link1BoxHalfExtents);
  // position
  btTransform link1BT;
  link1BT.setIdentity();
  link1BT.setOrigin(link1O);
  link1BT.setRotation(startQuatLink1_);
  // mass
  btScalar link1Mass = 1.0f;
  // create the link1 rigid body
  link1RB_ = createRigidBody(link1CS_,link1Mass,link1BT);
  // add it to the dynamics world
  dynamicsWorld_->addRigidBody(link1RB_);
  //--------end make link1 rigid body (the rotating arm near the pivot)-------//
  
  //----------make link0 rigid body (the sliding arm near the robot)---------//
  // shape
  const btVector3 link0BoxHalfExtents( link0Len_/2, sizeScale_, sizeScale_ );
  link0CS_ = new btBoxShape(link0BoxHalfExtents);
  // position
  btTransform link0BT;
  link0BT.setIdentity();
  link0BT.setOrigin(link0O);
  link0BT.setRotation(startQuatLink0_); // 90 from link1
  // mass
  btScalar link0Mass = 0.5f;
  // create the link0 rigid body
  link0RB_ = createRigidBody(link0CS_,link0Mass,link0BT);
  // add it to the dynamics world
  dynamicsWorld_->addRigidBody(link0RB_);
  //--------end make link0 rigid body (the sliding arm near the robot)-------//

  //--------make rbt rigid body (the robots manipulator)------//
  // shape
  const btVector3 rbtBoxHalfExtents( sizeScale_, 0.2f, sizeScale_ );
  rbtCS_ = new btBoxShape(rbtBoxHalfExtents);
  // position
  rbtBT_.setIdentity();
  rbtBT_.setOrigin(rbtO);
  rbtBT_.setRotation(startQuatLink0_); // same angle as link0
  // mass
  btScalar rbtMass = 0.1f;
  // create the rbt rigid body
  rbtRB_ = createRigidBody(rbtCS_,rbtMass,rbtBT_);
  // add it to the dynamics world
  dynamicsWorld_->addRigidBody(rbtRB_);
  //------end make rbt rigid body (the robots manipulator)----//

  // CREATE LATCH RIGID BODIES
  //----------make fxdA rigid body (the pivot)---------//
  // shape
  const btVector3 fxdABoxHalfExtents( sizeScale_, 4*sizeScale_, sizeScale_ );
  fxdACS_ = new btBoxShape(fxdABoxHalfExtents);
  // position
  btTransform fxdABT;
  fxdABT.setIdentity();
  fxdABT.setOrigin(fxdAO);
  fxdABT.setRotation(startQuatFxdABC_); // same angle as link0 in this case
  // mass
  btScalar fxdAMass = 0.0f; // fixed
  // create the fxdA rigid body
  fxdARB_ = createRigidBody(fxdACS_,fxdAMass,fxdABT);
  // add it to the dynamics world
  dynamicsWorld_->addRigidBody(fxdARB_);
  //--------end make fxdA rigid body (the pivot)-------//

  //----------make fxdB rigid body (the pivot)---------//
  // shape
  const btVector3 fxdBBoxHalfExtents( l1_/2, 4*sizeScale_, sizeScale_ );
  fxdBCS_ = new btBoxShape(fxdBBoxHalfExtents);
  // position
  btTransform fxdBBT;
  fxdBBT.setIdentity();
  fxdBBT.setOrigin(fxdBO);
  fxdBBT.setRotation(startQuatFxdABC_); // same angle as link0 in this case
  // mass
  btScalar fxdBMass = 0.0f; // fixed
  // create the fxdB rigid body
  fxdBRB_ = createRigidBody(fxdBCS_,fxdBMass,fxdBBT);
  // add it to the dynamics world
  dynamicsWorld_->addRigidBody(fxdBRB_);
  //--------end make fxdB rigid body (the pivot)-------//

  //----------make fxdC rigid body (the pivot)---------//
  // shape
  const btVector3 fxdCBoxHalfExtents( l1_/2, 4*sizeScale_, sizeScale_ );
  fxdCCS_ = new btBoxShape(fxdCBoxHalfExtents);
  // position
  btTransform fxdCBT;
  fxdCBT.setIdentity();
  fxdCBT.setOrigin(fxdCO);
  fxdCBT.setRotation(startQuatFxdABC_); // same angle as link0 in this case
  // mass
  btScalar fxdCMass = 0.0f; // fixed
  // create the fxdC rigid body
  fxdCRB_ = createRigidBody(fxdCCS_,fxdCMass,fxdCBT);
  // add it to the dynamics world
  dynamicsWorld_->addRigidBody(fxdCRB_);
  //--------end make fxdC rigid body (the pivot)-------//

  //--------------END OBJECT CREATION SECTION-----------------//

  //--------------CONSTRAINT CREATION SECTION-----------------//

  // Scope used in this section to avoid renaming local variables

  //--------create slider constraint between fxd and link1------//
  {
    btTransform frameInA = btTransform::getIdentity();
    btTransform frameInB = btTransform::getIdentity();
    bool useLinearReferenceFrameA = true;
    fxdLink1SC_ = new btSliderConstraint(*fxdRB_,*link1RB_,frameInA,frameInB,useLinearReferenceFrameA);
    // add sliding limits
    btScalar lower = -link1Len_/2; // HACK -link1Len_/2
    btScalar upper = link1Len_/2-2*sizeScale_; // HACK link1Len_/2
    fxdLink1SC_->setLowerLinLimit(lower);
    fxdLink1SC_->setUpperLinLimit(upper);
    // add constraint to the world
    const bool isDisableCollisionsBetweenLinkedBodies = true; //disable collisions
    dynamicsWorld_->addConstraint(fxdLink1SC_,isDisableCollisionsBetweenLinkedBodies);
  }
  //------end create hinge constraint between fxd and link1----//

  //--------create 90 slider constraint between link1 and link0------//
  {
    btTransform frameInA = btTransform::getIdentity();
    btTransform frameInB = btTransform::getIdentity();
    const btVector3 frameInAOrigin( o2_, 0.0f, 0.0f );
    double th_frameInA = 1.5708;
    const btQuaternion frameInAQuat(0.0f, sin(0.5f*th_frameInA), 0.0f, cos(0.5f*th_frameInA));
    frameInA.setOrigin(frameInAOrigin);
    frameInA.setRotation(frameInAQuat);
    bool useLinearReferenceFrameA = true;
    link1Link0SC_ = new btSliderConstraint(*link1RB_,*link0RB_,frameInA,frameInB,useLinearReferenceFrameA);
    // add sliding limits
    btScalar lower = -link0Len_/2; // HACK -link0Len_/2-2*sizeScale_
    btScalar upper = link0Len_/2-2*sizeScale_; // HACK link0Len_/2
    link1Link0SC_->setLowerLinLimit(lower);
    link1Link0SC_->setUpperLinLimit(upper);
    // add constraint to the world
    const bool isDisableCollisionsBetweenLinkedBodies = true; //disable collisions
    dynamicsWorld_->addConstraint(link1Link0SC_,isDisableCollisionsBetweenLinkedBodies);
  }
  //------end create 90 slider constraint between link1 and link0----//
  
  //--------create hinge constraint between link0 and rbt------//
  {
    const btVector3 pivotInA( o1_, 0.0f, 0.0f );   
    const btVector3 pivotInB( 0.0f, 0.0f, 0.0f );
    btVector3 axisInA( 0.0f, 1.0f, 0.0f );
    btVector3 axisInB( 0.0f, 1.0f, 0.0f );
    bool useReferenceFrameA = false;
    link0RbtHC_ = new btHingeConstraint(*link0RB_,*rbtRB_,pivotInA,pivotInB,axisInA,axisInB,useReferenceFrameA);
    // add constraint to the world
    const bool isDisableCollisionsBetweenLinkedBodies = true; //disable collisions
    dynamicsWorld_->addConstraint(link0RbtHC_,isDisableCollisionsBetweenLinkedBodies);
  }
  //------end create hinge constraint between link0 and rbt----//

  //--------------END CONSTRAINT CREATION SECTION-----------------//

  //---------INITIALIZE REMAINING PARAMETERS SECTION----------//

  // Disable deactivation for every object
  fxdRB_->forceActivationState(DISABLE_DEACTIVATION);
  link1RB_->forceActivationState(DISABLE_DEACTIVATION);
  link0RB_->forceActivationState(DISABLE_DEACTIVATION);
  rbtRB_->forceActivationState(DISABLE_DEACTIVATION);

  fxdARB_->forceActivationState(DISABLE_DEACTIVATION);
  fxdBRB_->forceActivationState(DISABLE_DEACTIVATION);
  fxdCRB_->forceActivationState(DISABLE_DEACTIVATION);

  fxdRB_->setSleepingThresholds(0.0,0.0);
  link1RB_->setSleepingThresholds(0.0,0.0);
  link0RB_->setSleepingThresholds(0.0,0.0);
  rbtRB_->setSleepingThresholds(0.0,0.0);

  fxdARB_->setSleepingThresholds(0.0,0.0);
  fxdBRB_->setSleepingThresholds(0.0,0.0);
  fxdCRB_->setSleepingThresholds(0.0,0.0);

  // change friction - don't think this is necessary for this mechanism
  fxdRB_->setFriction(0.1);
  link1RB_->setFriction(0.1);
  link0RB_->setFriction(0.1);
  rbtRB_->setFriction(0.1);

  fxdARB_->setFriction(0.1);
  fxdBRB_->setFriction(0.1);
  fxdCRB_->setFriction(0.1);

  // ALWAYS call updatePose() before starting to initiliaze pose_ and rbtBT_
  updatePose();

  //-------END INITIALIZE REMAINING PARAMETERS SECTION-------//
}

// virtual
void MechPrisPrisL::exitPhysics(){
  // cleanup in the reverse order of creation/initialization
  // remove additional things created in this subclass
  // all rigid bodies are taken care of in the superclass
  delete fxdLink1SC_;
  delete link1Link0SC_;
  delete link0RbtHC_;

  delete link0CS_;
  delete link1CS_;
  delete fxdCS_;

  delete fxdACS_;
  delete fxdBCS_;
  delete fxdCCS_;
}

////////////////////////////////////////////////////////////////////////////////
//                           End Redefined Section                            //
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//                         Sub (Derived) Class Section                        //
////////////////////////////////////////////////////////////////////////////////

// virtual
MechPrisPrisL::~MechPrisPrisL(){
  exitPhysics();
  // This should then call Mechanism::~Mechanism() automatically
}

// Specific to latch mechanisms
bool MechPrisPrisL::inContact(){
  if (dynamicsWorld_){
    dynamicsWorld_->performDiscreteCollisionDetection();
    ///one way to draw all the contact points is iterating over contact manifolds in the dispatcher:  
    int numManifolds = dynamicsWorld_->getDispatcher()->getNumManifolds();
    for (size_t i=0;i<numManifolds;i++)
      {
	btPersistentManifold* contactManifold = dynamicsWorld_->getDispatcher()->getManifoldByIndexInternal(i);
	
	int numContacts = contactManifold->getNumContacts();
	if (numContacts > 0){
	  //std::cout << "INVALID" << std::endl;
	  return true;
	}
      }
    //std::cout << "VALID" << std::endl;
    return false;
  }
  //else std::cout << "There is no world to check collisions in" << std::endl;
}

// Not Defined in super (base) class
// virtual
void MechPrisPrisL::setStartWithState(stateStruct& startState){
  // State looks like:
  // Model: 5
  // Params: x_axis2,y_axis2,theta_axis2 in rbt space, d_L2, d_L1
  // Vars: d_2, d_1
  
  // Create an x,y,z vector in rbt for rbt, link0, fxd (object_frame)
  std::vector<double> tempVarsRbt_rbt (3,0.0);
  std::vector<double> tempVarsLink0_rbt (3,0.0);
  std::vector<double> tempVarsLink1_rbt (3,0.0);
  std::vector<double> tempVarsFxd_rbt (3,0.0);

  // Fixed pieces of the latch
  std::vector<double> tempVarsFxdA_rbt (3,0.0);
  std::vector<double> tempVarsFxdB_rbt (3,0.0);
  std::vector<double> tempVarsFxdC_rbt (3,0.0);
  
  double d_2 = startState.vars[0];
  double d_1 = startState.vars[1];

  x_a2_ = startState.params[0];
  y_a2_ = startState.params[1];
  theta_a2_ = startState.params[2];
  d_L2_ = startState.params[3];
  d_L1_ = startState.params[4];

  // Set needed geometry variables
  double theta_a1 = theta_a2_ + 1.5708; // always add pi/2 for now
  link0Len_ = 2*d_L1_; // length of link connected to robot
  link1Len_ = 2*d_L2_; // length of link connected to fixed pivot
  o1_ = link0Len_/2+2*sizeScale_;
  o2_ = link1Len_/2+2*sizeScale_;

  // Calculate
  // Rbt
  tempVarsRbt_rbt[0] = d_2*cos(theta_a2_)-d_1*sin(theta_a2_)+x_a2_; // set x_rbt
  tempVarsRbt_rbt[1] = d_2*sin(theta_a2_)+d_1*cos(theta_a2_)+y_a2_; // set y_rbt
  tempVarsRbt_rbt[2] = 1.0; // set z_rbt to 1.0

  // Link0
  tempVarsLink0_rbt[0] = d_2*cos(theta_a2_)-(d_1-o1_)*sin(theta_a2_)+x_a2_; // set x_link0
  tempVarsLink0_rbt[1] = d_2*sin(theta_a2_)+(d_1-o1_)*cos(theta_a2_)+y_a2_; // set y_link0
  tempVarsLink0_rbt[2] = 1.0; // set z_link0 to 1.0

  // Link1
  tempVarsLink1_rbt[0] = (d_2-o2_)*cos(theta_a2_)+x_a2_; // set x_link1
  tempVarsLink1_rbt[1] = (d_2-o2_)*sin(theta_a2_)+y_a2_; // set y_link1
  tempVarsLink1_rbt[2] = 1.0; // set z_link1 to 1.0

  // Fxd
  tempVarsFxd_rbt[0] = x_a2_; // set x_fxd
  tempVarsFxd_rbt[1] = y_a2_; // set y_fxd
  tempVarsFxd_rbt[2] = 1.0; // set z_fxd to 1.0

  // Latch Objects
  // FxdA
  tempVarsFxdA_rbt[0] = d_L2_*cos(theta_a2_)-(d_L1_+2*sizeScale_+o3_)*sin(theta_a2_)+x_a2_; // set x_fxdA
  tempVarsFxdA_rbt[1] = d_L2_*sin(theta_a2_)+(d_L1_+2*sizeScale_+o3_)*cos(theta_a2_)+y_a2_; // set y_fxdA
  tempVarsFxdA_rbt[2] = 1.0; // set z_fxdA to 1.0

  // FxdB
  tempVarsFxdB_rbt[0] = (d_L2_+2*sizeScale_+o3_)*cos(theta_a2_)-(d_L1_+3*sizeScale_+o3_-l1_/2)*sin(theta_a2_)+x_a2_; // set x_fxdB
  tempVarsFxdB_rbt[1] = (d_L2_+2*sizeScale_+o3_)*sin(theta_a2_)+(d_L1_+3*sizeScale_+o3_-l1_/2)*cos(theta_a2_)+y_a2_; // set y_fxdB
  tempVarsFxdB_rbt[2] = 1.0; // set z_fxdB to 1.0

  // FxdC
  tempVarsFxdC_rbt[0] = (d_L2_-2*sizeScale_-o3_)*cos(theta_a2_)-(d_L1_+3*sizeScale_+o3_-l1_/2)*sin(theta_a2_)+x_a2_; // set x_fxdC
  tempVarsFxdC_rbt[1] = (d_L2_-2*sizeScale_-o3_)*sin(theta_a2_)+(d_L1_+3*sizeScale_+o3_-l1_/2)*cos(theta_a2_)+y_a2_; // set y_fxdC
  tempVarsFxdC_rbt[2] = 1.0; // set z_fxdC to 1.0

  // Convert to sim and set
  std::vector<double> tempVarsRbt_sim = convCoordsRbtToSim(tempVarsRbt_rbt);
  std::vector<double> tempVarsLink0_sim = convCoordsRbtToSim(tempVarsLink0_rbt);
  std::vector<double> tempVarsLink1_sim = convCoordsRbtToSim(tempVarsLink1_rbt);
  std::vector<double> tempVarsFxd_sim = convCoordsRbtToSim(tempVarsFxd_rbt);

  // fixed latch objects
  std::vector<double> tempVarsFxdA_sim = convCoordsRbtToSim(tempVarsFxdA_rbt);
  std::vector<double> tempVarsFxdB_sim = convCoordsRbtToSim(tempVarsFxdB_rbt);
  std::vector<double> tempVarsFxdC_sim = convCoordsRbtToSim(tempVarsFxdC_rbt);

  // Set class member variables
  startPose_ = convStdVectToBtVect3(tempVarsRbt_sim);
  startPoseLink0_ = convStdVectToBtVect3(tempVarsLink0_sim);
  startPoseLink1_ = convStdVectToBtVect3(tempVarsLink1_sim);
  startPoseFxd_ = convStdVectToBtVect3(tempVarsFxd_sim);
  
  // fixed latch objects
  startPoseFxdA_ = convStdVectToBtVect3(tempVarsFxdA_sim);
  startPoseFxdB_ = convStdVectToBtVect3(tempVarsFxdB_sim);
  startPoseFxdC_ = convStdVectToBtVect3(tempVarsFxdC_sim);

  // theta,theta_L in rbt. quaternion in sim.
  startQuatLink0_.setValue(0.0f, sin(0.5f*theta_a1), 0.0f, cos(0.5f*theta_a1));
  startQuatLink1_.setValue(0.0f, sin(0.5f*theta_a2_), 0.0f, cos(0.5f*theta_a2_));
  startQuatFxdABC_ = startQuatLink0_;
}

/*
// used to be virtual
void MechPrisPrisL::setGoalWithAction(std::vector<double>& action){
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
stateStruct MechPrisPrisL::returnStateOfWorld(){
  // State looks like:
  // Model: 5
  // Params: x_axis2,y_axis2,theta_axis2 in rbt space, d_L2, d_L1
  // Vars: d_2, d_1

  stateStruct endState;
  endState.model = 5;
  endState.params.push_back(x_a2_);
  endState.params.push_back(y_a2_);
  endState.params.push_back(theta_a2_);
  endState.params.push_back(d_L2_);
  endState.params.push_back(d_L1_);

  std::vector<double> tempVarsSim = convBtVect3ToStdVect(pose_);
  std::vector<double> tempVarsRbt = convCoordsSimToRbt(tempVarsSim);
  // substitute equations for x_r and y_r to calculate d_2 and d_1
  double d_2 = (tempVarsRbt[1]-y_a2_)*sin(theta_a2_)+(tempVarsRbt[0]-x_a2_)*cos(theta_a2_);
  double d_1 = (tempVarsRbt[1]-y_a2_)*cos(theta_a2_)-(tempVarsRbt[0]-x_a2_)*sin(theta_a2_);
  endState.vars.push_back(d_2);
  endState.vars.push_back(d_1);
  return endState;
}

// virtual
std::vector<double> MechPrisPrisL::stToObs(stateStruct& state){
  // State looks like:
  // Model: 5
  // Params: x_axis2,y_axis2,theta_axis2 in rbt space, d_L2, d_L1
  // Vars: d_2, d_1

  double d_2 = state.vars[0];
  double d_1 = state.vars[1];
  double x_a2 = state.params[0];
  double y_a2 = state.params[1];
  double theta_a2 = state.params[2];
  
  // Observation looks like:
  // x,y in rbt space
  std::vector<double> obs (2,0.0);

  // Calculate
  obs[0] = x_a2+d_2*cos(theta_a2)-d_1*sin(theta_a2); // set x_obs
  obs[1] = y_a2+d_2*sin(theta_a2)+d_1*cos(theta_a2); // set y_obs
  return obs;
}

// virtual
std::vector<double> MechPrisPrisL::stToRbt(stateStruct& state){
  // State looks like:
  // Model: 5
  // Params: x_axis2,y_axis2,theta_axis2 in rbt space, d_L2, d_L1
  // Vars: d_2, d_1

  double d_2 = state.vars[0];
  double d_1 = state.vars[1];
  double x_a2 = state.params[0];
  double y_a2 = state.params[1];
  double theta_a2 = state.params[2];

  // Robot looks like:
  // x,y in rbt space
  std::vector<double> rbt (2,0.0);

  // Calculate
  rbt[0] = x_a2+d_2*cos(theta_a2)-d_1*sin(theta_a2); // set x_obs
  rbt[1] = y_a2+d_2*sin(theta_a2)+d_1*cos(theta_a2); // set y_obs
  return rbt;
}

bool MechPrisPrisL::isStateValid(stateStruct& state,std::vector< std::vector<double> >& workspace){
  // State looks like:
  // Model: 5
  // Params: x_axis2,y_axis2,theta_axis2 in rbt space, d_L2, d_L1
  // Vars: d_2, d_1
  
  // Multiple conditions to check in order of increasing cost of computation
  double d_2 = state.vars[0];
  double d_1 = state.vars[1];
  double d_L2 = state.params[3];
  double d_L1 = state.params[4];
  // Check if state violates the basic rules of this latch
  if (d_2<0 || d_2>2*d_L2 || d_1<0 || d_1>2*d_L1 || d_L2<=0 || d_L1<=0){ /*std::cout << "error 1" << std::endl;*/ return false;}
  else{
    // Check if state places rbt outside of rbt workspace
    std::vector<double> rbt = stToRbt(state);
    if (rbt[0]<workspace[0][0] || rbt[0]>workspace[0][1] || rbt[1]<workspace[1][0] || rbt[1]>workspace[1][1]){ /*std::cout << "error 2" << std::endl;*/ return false;}
    else{
      // Check if state causes collision on initialization
      initialize(state);
      if (inContact()){ /*std::cout << "error 3" << std::endl;*/ return false;}
      else return true;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
//                       End Sub (Derived) Class Section                      //
////////////////////////////////////////////////////////////////////////////////
